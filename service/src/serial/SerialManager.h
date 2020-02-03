//
// Created by Kishchenko, Ivan on 2019-08-27.
//

#ifndef ROVER_SERIALMANAGER_H
#define ROVER_SERIALMANAGER_H

#include <serial/serial.h>
#include <rpc/RpcMethod.h>
#include <subsystem/SubsystemBase.h>
#include <rpc/RpcRegistry.h>

#include <map>
#include <string>
#include <thread>
#include <mutex>
#include <subsystem/config/Config.h>

class SerialManager :
        public SubsystemBase,
        public RpcRegistry,
        public std::enable_shared_from_this<SerialManager> {
public:
    typedef std::shared_ptr<SerialManager> Ptr;
    typedef std::string handler;
    typedef std::shared_ptr<serial::Serial> DevicePtr;

    struct DeviceCtrlInfo {
        DevicePtr device;
        std::unique_ptr<std::thread> thread;
    };

    const char *name() const override {
        return "serialManager";
    }

    void postConstruct(Application &app) override;

    std::vector<serial::PortInfo> getFoundDevices();

    handler openDevice(const std::string &device, uint32_t baud);

    size_t writeDevice(const SerialManager::handler &device, const std::string &data);

    size_t writeDevice(const SerialManager::handler &device, const uint8_t *data, size_t size);

    size_t writeDevice(const SerialManager::handler &device, uint8_t data) {
        return writeDevice(device, (uint8_t *) &data, sizeof(uint8_t));
    }

    size_t writeDevice(const SerialManager::handler &device, uint16_t data) {
        return writeDevice(device, (uint8_t *) &data, sizeof(uint16_t));
    }

    size_t writeDevice(const SerialManager::handler &device, uint32_t data) {
        return writeDevice(device, (uint8_t *) &data, sizeof(uint32_t));
    }

    size_t writeDevice(const SerialManager::handler &device, int8_t data) {
        return writeDevice(device, (uint8_t *) &data, sizeof(int8_t));
    }

    size_t writeDevice(const SerialManager::handler &device, int16_t data) {
        return writeDevice(device, (uint8_t *) &data, sizeof(int16_t));
    }

    size_t writeDevice(const SerialManager::handler &device, int32_t data) {
        return writeDevice(device, (uint8_t *) &data, sizeof(int32_t));
    }

    size_t readDevice(const SerialManager::handler &device, uint8_t *data, size_t size);

    std::string readDevice(const SerialManager::handler &device);

    void closeDevice(const handler &device);

private:
    std::mutex _mutexDevice;
    std::map<std::string, DeviceCtrlInfo> _openDevices;
};


#endif //ROVER_SERIALMANAGER_H
