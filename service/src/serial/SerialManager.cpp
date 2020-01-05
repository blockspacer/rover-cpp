//
// Created by Kishchenko, Ivan on 2019-08-27.
//

#include "SerialManager.h"
#include "SerialRpc.h"

#include <vector>
#include <iostream>
#include <memory>
#include <boost/lexical_cast.hpp>

void SerialManager::postConstruct(Application &app) {
    addMethod(std::make_shared<SerialRpcDevices>(shared_from_this()));
    addMethod(std::make_shared<SerialRpcOpenDevice>(shared_from_this()));
    addMethod(std::make_shared<SerialRpcCloseDevice>(shared_from_this()));
    addMethod(std::make_shared<SerialRpcWriteDevice>(shared_from_this()));
    addMethod(std::make_shared<SerialRpcInitDevice>(shared_from_this()));

    SubsystemBase::postConstruct(app);
}

std::vector<serial::PortInfo> SerialManager::getFoundDevices() {
    return serial::list_ports();
}

SerialManager::handler SerialManager::openDevice(const std::string &port, uint32_t baud) {
    std::lock_guard<std::mutex> guard(_mutexDevice);

    if (_openDevices.find(port) != _openDevices.end()) {
        return port;
    }

    DevicePtr device = std::make_shared<serial::Serial>(port, baud, serial::Timeout::simpleTimeout(1000));
    if (device->isOpen()) {
        DeviceCtrlInfo info;
        info.device = device;
        this->info(port + " available " + std::to_string(device->available()));
        device->read(device->available());

        /*
        info.thread.reset(new std::thread([device, this]() {
                              std::string data;
                              while (device->isOpen()) {
                                  if (device->waitReadable()) {
                                      data += device->read(device->available());
                                      auto pos = data.find("\r\n");
                                      if (pos != std::string::npos) {
                                          this->info("Serial: " + data.substr(0, pos));
                                          data.erase(0, pos+2);
                                      }
                                  }
                              }
                          })
        );
         */

        _openDevices[port] = std::move(info);

        return port;
    }

    return port;
}

size_t SerialManager::writeDevice(const SerialManager::handler &device, const std::string &data) {
    std::lock_guard<std::mutex> guard(_mutexDevice);

    const auto it = _openDevices.find(device);
    if (it != _openDevices.end()) {
        return it->second.device->write(data);
    }

    return -1;
}

size_t SerialManager::writeDevice(const SerialManager::handler &device, const uint8_t *data, size_t size) {
    std::lock_guard<std::mutex> guard(_mutexDevice);

    const auto it = _openDevices.find(device);
    if (it != _openDevices.end()) {
        return it->second.device->write(data, size);
    }

    return -1;
}

size_t SerialManager::readDevice(const SerialManager::handler &device, uint8_t *data, size_t size) {
    std::lock_guard<std::mutex> guard(_mutexDevice);

    const auto it = _openDevices.find(device);
    if (it != _openDevices.end()) {
        int cnt = 0;
        while (it->second.device->available() < size) {
            if (cnt++ > 3) {
                throw std::underflow_error("read timeout");
            }
            sleep(1);
        };
        info(device + " available " + std::to_string(it->second.device->available()));

        return it->second.device->read(data, size);
    }

    return -1;
}

std::string SerialManager::readDevice(const SerialManager::handler &device) {
    std::lock_guard<std::mutex> guard(_mutexDevice);

    const auto it = _openDevices.find(device);
    if (it != _openDevices.end()) {
        it->second.device->readline();
        return it->second.device->readline();
    }

    return "";

}

void SerialManager::closeDevice(const SerialManager::handler &device) {
    std::lock_guard<std::mutex> guard(_mutexDevice);

    const auto it = _openDevices.find(device);
    if (it != _openDevices.end()) {
        it->second.device->close();
        //it->second.thread->join();

        _openDevices.erase(device);
    }
}
