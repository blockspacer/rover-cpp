//
// Created by Ivan Kishchenko on 02/09/2019.
//

#ifndef ROVER_SERIALRPC_H
#define ROVER_SERIALRPC_H

#include <rpc/RpcMethod.h>
#include <rpc/json/Helper.h>

#include "serial/SerialManager.h"
#include <string.h>

BEGIN_DECLARE_DTO(SerialPortInfo)

__DECLARE_DTO_FIELDS3(
        std::string, port,
        std::string, description,
        std::string, hardwareId
)
END_DECLARE_DTO

typedef std::vector<SerialPortInfo> SerialPortInfoVec;

BEGIN_DECLARE_DTO(PortList)

__DECLARE_DTO_FIELDS1(std::vector<SerialPortInfo>, ports)
END_DECLARE_DTO

class SerialRpcDevices : public RpcSupplier<SerialPortInfoVec> {
private:
    SerialManager::Ptr _serialManager;
public:

    explicit SerialRpcDevices(const SerialManager::Ptr serialManager) : _serialManager(serialManager) {}

    std::string name() const override {
        return "serial.devices";
    }

    SerialPortInfoVec exec() const override {
        SerialPortInfoVec result;
        for (const auto &device : _serialManager->getFoundDevices()) {
            SerialPortInfo portInfo;
            portInfo.port = device.port;
            portInfo.description = device.description;
            portInfo.hardwareId = device.hardware_id;

            result.push_back(portInfo);
        }

        return result;
    }
};

BEGIN_DECLARE_DTO(SerialOpenDeviceParams)

__DECLARE_DTO_FIELDS2(
        std::string, port,
        uint32_t, baud
)
END_DECLARE_DTO


class SerialRpcOpenDevice : public RpcFunction<SerialOpenDeviceParams, std::string> {
private:
    SerialManager::Ptr _serialManager;
public:

    explicit SerialRpcOpenDevice(const SerialManager::Ptr serialManager) : _serialManager(serialManager) {}

    std::string name() const override {
        return "serial.open";
    }

    std::string exec(const SerialOpenDeviceParams &params) const override {
        return _serialManager->openDevice(params.port, params.baud);
    }
};

class SerialRpcCloseDevice : public RpcConsumer<std::string> {
private:
    SerialManager::Ptr _serialManager;
public:

    explicit SerialRpcCloseDevice(const SerialManager::Ptr serialManager) : _serialManager(serialManager) {}

    std::string name() const override {
        return "serial.close";
    }

    void exec(const std::string &params) const override {
        _serialManager->closeDevice(params);
    }

};

BEGIN_DECLARE_DTO(SerialWriteDeviceParams)

__DECLARE_DTO_FIELDS2(
        std::string, port,
        std::string, data
)
END_DECLARE_DTO

class SerialRpcWriteDevice : public RpcConsumer<SerialWriteDeviceParams> {
private:
    SerialManager::Ptr _serialManager;
public:

    explicit SerialRpcWriteDevice(const SerialManager::Ptr serialManager) : _serialManager(serialManager) {}

    std::string name() const override {
        return "serial.write";
    }

    void exec(const SerialWriteDeviceParams &params) const override {
        _serialManager->writeDevice(params.port, params.data);
    }

};

typedef std::vector<uint8_t> Buffer;

class SerialRpcInitDevice : public RpcConsumer<std::string> {
private:
    SerialManager::Ptr _serialManager;
public:

    explicit SerialRpcInitDevice(const SerialManager::Ptr serialManager) : _serialManager(serialManager) {}

    std::string name() const override {
        return "serial.init";
    }

    /*
    static Buffer push(Buffer &buffer, protocol::Packet &packet, size_t size) {
        size_t pos = buffer.size();
        buffer.resize(pos + size);

        memcpy((void *) (buffer.data() + pos), (const void *) &packet, size);

        return buffer;
    }
     */

    void exec(const std::string &device) const override {
        /*
        protocol::Mode mode(13, protocol::Output);
        protocol::DigitalWrite switchOn(13, true);
        protocol::Delay delay(5000);
        protocol::DigitalWrite switchOff(13, false);
        protocol::Message message("done");

        size_t size = 256;
        uint8_t data[256];
        uint8_t code;

        size_t base = 0;
        size_t res = 0;

        base = delay.pack(data, 256);
        res = _serialManager->writeDevice(device, data, base);
        _serialManager->readDevice(device, (uint8_t*)&code, sizeof(code));

        base = mode.pack(data, 256);
        res = _serialManager->writeDevice(device, data, base);
        _serialManager->readDevice(device, (uint8_t*)&code, sizeof(uint16_t));

        base = switchOn.pack(data, 256);
        res = _serialManager->writeDevice(device, data, base);
        _serialManager->readDevice(device, (uint8_t*)&code, sizeof(uint16_t));

        base = switchOff.pack(data, 256);
        res = _serialManager->writeDevice(device, data, base);
        _serialManager->readDevice(device, (uint8_t*)&code, sizeof(uint16_t));

        base = mode.pack(data, size);
        base += switchOn.pack(data + base, size - base);
        base += delay.pack(data + base, size - base);
        base += switchOff.pack(data + base, size - base);
        delay.delay = 1000;
        base += delay.pack(data + base, size - base);
        //base += message.pack(data + base, size-base);

        protocol::RegisterBatch batch(0, data, base);
        uint8_t batchData[256];
        base = batch.pack(batchData, 256);
        res = _serialManager->writeDevice(device, batchData, base);

        _serialManager->readDevice(device, (uint8_t *) &code, sizeof(code));
        if (code == batch.code) {
            protocol::ExecBatch exec(0);
            base = exec.pack(batchData, 256);
            res = _serialManager->writeDevice(device, batchData, base);
            _serialManager->readDevice(device, (uint8_t *) &code, sizeof(code));
        }
        */
    }

};


#endif //ROVER_SERIALRPC_H
