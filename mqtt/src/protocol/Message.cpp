//
// Created by Ivan Kishchenko on 24.11.2019.
//

#include <ostream>
#include <istream>
#include <assert.h>
#include "Message.h"
#include "Protocol.h"

namespace mqtt {
    ConnectMessage::ConnectMessage() {
        header_.bits.type = mqtt::MQTT_MSG_CONNECT;
        header_.bits.dup = false;
        header_.bits.qos = 0;
        header_.bits.retain = false;

        flags_.bits.username = false;
        flags_.bits.password = false;
        flags_.bits.willRetain = false;
        flags_.bits.willQoS = 0;
        flags_.bits.will = false;
        flags_.bits.cleanStart = false;
    }

    void ConnectMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        Protocol::packByte(stream, header_.all);
        int calcLen = 8 + 1 + 1 + 2 + 7;
        Protocol::packVariableInt(stream, calcLen);

        size_t res = 0;
        /// 3.1.2 Variable header
        /// 3.1.2.1 Protocol name
        res += Protocol::packString(stream, "MQIsdp");
        /// 3.1.2.2 Protocol version
        res += Protocol::packByte(stream, (uint8_t) 3);
        /// 3.1.2.2 Protocol flags
        res += Protocol::packByte(stream, flags_.all);
        // ...
        /// 3.1.2.10 Keep alive
        res += Protocol::packWord(stream, (uint16_t) 10);

        /// 3.1.3 Payload
        /// 3.1.3.1 Client Id
        res += Protocol::packString(stream, "rover");

        assert(calcLen == res);
    }

    void ConnectMessage::decode(boost::asio::streambuf &buf) {

    }


    void ConnAckMessage::encode(boost::asio::streambuf &buf) {

    }

    void ConnAckMessage::decode(boost::asio::streambuf &buf) {
        boost::asio::streambuf cache;
        cache.commit(
                buffer_copy(
                        cache.prepare(buf.size()), // target's output sequence
                        buf.data()
                )
        );
        std::istream stream(&cache);
        stream.exceptions(std::istream::eofbit | std::istream::badbit);

        header_.all = Protocol::unPackByte(stream);
        assert(header_.bits.type == mqtt::MQTT_MSG_CONNACK);

        size_t len = Protocol::unPackVariableInt(stream);

        if (len > buf.size()) {
            throw;
        }

        Protocol::unPackByte(stream);
        rc_ = Protocol::unPackByte(stream);

        buf.consume(buf.size() - cache.size());
    }

    PingReqMessage::PingReqMessage() {
        header_.bits.type = mqtt::MQTT_MSH_PINGREQ;
        header_.bits.dup = false;
        header_.bits.qos = 0;
        header_.bits.retain = false;

    }

    void PingReqMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        Protocol::packByte(stream, header_.all);
        Protocol::packVariableInt(stream, 0);
    }

    void PingReqMessage::decode(boost::asio::streambuf &buf) {

    }

    PingRespMessage::PingRespMessage() {
        header_.bits.type = mqtt::MQTT_MSH_PINGRESP;
        header_.bits.dup = false;
        header_.bits.qos = 0;
        header_.bits.retain = false;
    }

    void PingRespMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        Protocol::packByte(stream, header_.all);
        Protocol::packVariableInt(stream, 0);
    }

    void PingRespMessage::decode(boost::asio::streambuf &buf) {
        boost::asio::streambuf cache;
        cache.commit(
                buffer_copy(
                        cache.prepare(buf.size()), // target's output sequence
                        buf.data()
                )
        );
        std::istream stream(&cache);
        stream.exceptions(std::istream::eofbit | std::istream::badbit);

        header_.all = Protocol::unPackByte(stream);
        assert(header_.bits.type == mqtt::MQTT_MSH_PINGRESP);

        size_t len = Protocol::unPackVariableInt(stream);
        assert(len == 0);

        buf.consume(buf.size() - cache.size());
    }

    SubscribeMessage::SubscribeMessage(uint16_t  msgId, const std::string& topic) : topic_(topic) {
        header_.bits.type = mqtt::MQTT_MSH_SUBSCRIBE;
        header_.bits.dup = false;
        header_.bits.qos = 1;
        header_.bits.retain = false;
    }

    void SubscribeMessage::encode(boost::asio::streambuf& buf) {
        std::ostream stream(&buf);

        Protocol::packByte(stream, header_.all);
        int len = 2 + 6 + 1;
        Protocol::packVariableInt(stream, len);
        Protocol::packWord(stream, msgId_);
        Protocol::packString(stream, topic_);
        Protocol::packByte(stream, 0x02);
    }

    void SubscribeMessage::decode(boost::asio::streambuf& buf) {

    }

    SubsAckMessage::SubsAckMessage() {
        header_.bits.type = mqtt::MQTT_MSH_SUBSACK;
        header_.bits.dup = false;
        header_.bits.qos = 0;
        header_.bits.retain = false;
    }

    void SubsAckMessage::encode(boost::asio::streambuf& buf) {

    }

    void SubsAckMessage::decode(boost::asio::streambuf& buf) {

    }

}
