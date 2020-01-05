//
// Created by Ivan Kishchenko on 24.11.2019.
//

#include <ostream>
#include <istream>
#include <assert.h>
#include "Message.h"
#include "Protocol.h"
#include "Packet.h"

namespace mqtt {
    Message::Message(unsigned int type) {
        _type = type;
        header_.bits.type = type;
        header_.bits.dup = false;
        header_.bits.qos = 0;
        header_.bits.retain = false;
    }

    void Message::pack(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);
        Protocol::packByte(stream, header_.all);
        boost::asio::streambuf data;
        encode(data);

        Protocol::packVariableInt(stream, data.size());
        stream.write((const char *) data.data().data(), data.size());
    }

    void Message::unPack(boost::asio::streambuf &buf) {
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
        assert(_type == header_.bits.type);
        size_t len = Protocol::unPackVariableInt(stream);

        if (len > buf.size()) {
            throw;
        }

        cache.consume(stream.tellg());
        decode(cache);
        buf.consume(buf.size() - cache.size());
    }

    ConnectMessage::ConnectMessage() : Message(mqtt::MQTT_MSG_CONNECT) {
        flags_.bits.username = false;
        flags_.bits.password = false;
        flags_.bits.willRetain = false;
        flags_.bits.willQoS = 0;
        flags_.bits.will = false;
        flags_.bits.cleanStart = false;
    }

    void ConnectMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);
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

        assert(res == buf.size());
    }

    void ConnectMessage::decode(boost::asio::streambuf &buf) {

    }


    void ConnAckMessage::encode(boost::asio::streambuf &buf) {

    }

    ConnAckMessage::ConnAckMessage() : Message(mqtt::MQTT_MSG_CONNACK) {

    }

    void ConnAckMessage::decode(boost::asio::streambuf &buf) {
        std::istream stream(&buf);
        Protocol::unPackByte(stream);
        rc_ = Protocol::unPackByte(stream);
    }

    PingReqMessage::PingReqMessage() : Message(mqtt::MQTT_MSH_PINGREQ) {
    }

    PingRespMessage::PingRespMessage() : Message(mqtt::MQTT_MSH_PINGRESP) {
    }

    SubscribeMessage::SubscribeMessage(uint16_t msgId, std::string_view topic)
            : Message(mqtt::MQTT_MSH_SUBSCRIBE), msgId_(msgId), topic_(topic) {
    }

    void SubscribeMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        /// 3.8.2 Variable header
        /// 3.8.2.1 Variable header
        Protocol::packWord(stream, msgId_);

        /// 3.8.3 Payload
        Protocol::packString(stream, topic_);
        Protocol::packByte(stream, 0x02);
    }

    void SubscribeMessage::decode(boost::asio::streambuf &buf) {

    }

    SubsAckMessage::SubsAckMessage() : Message(mqtt::MQTT_MSH_SUBSACK) {
    }

    void SubsAckMessage::encode(boost::asio::streambuf &buf) {

    }

    void SubsAckMessage::decode(boost::asio::streambuf &buf) {
        std::istream stream(&buf);

        /// 3.9.2 Variable header
        msgId_ = Protocol::unPackWord(stream);

        /// 3.9.3 Payload
        rc_ = Protocol::unPackByte(stream);
    }

    UnSubscribeMessage::UnSubscribeMessage(uint16_t msgId, std::string_view topic)
            : Message(mqtt::MQTT_MSH_UNSUBSCRIBE), msgId_(msgId), topic_(topic) {
    }

    void UnSubscribeMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        /// 3.10.2 Variable header
        Protocol::packWord(stream, msgId_);

        /// 3.10.3 Payload
        Protocol::packString(stream, topic_);
        Protocol::packByte(stream, 0x02);
    }

    void UnSubscribeMessage::decode(boost::asio::streambuf &buf) {

    }

    UnSubAckMessage::UnSubAckMessage() : Message(mqtt::MQTT_MSH_UNSUBSACK) {
    }

    void UnSubAckMessage::encode(boost::asio::streambuf &buf) {

    }

    void UnSubAckMessage::decode(boost::asio::streambuf &buf) {
        std::istream stream(&buf);

        /// 3.10.2 Variable header
        msgId_ = Protocol::unPackWord(stream);

        /// 3.10.3 Payload
        rc_ = Protocol::unPackByte(stream);
    }

    DisconnectMessage::DisconnectMessage() : Message(mqtt::MQTT_MSH_DISCONNECT) {
    }

    PublishMessage::PublishMessage(uint16_t msgId, std::string_view topic, const std::byte *data, std::size_t size)
            : Message(mqtt::MQTT_MSH_PUBLISH), msgId_(msgId), topic_(topic) {
        buf_.resize(size);
        std::memcmp(buf_.data(), data, size);
    }

    PublishMessage::PublishMessage(uint16_t msgId, std::string_view topic)
            : Message(mqtt::MQTT_MSH_PUBLISH), msgId_(msgId), topic_(topic) {

    }

    void PublishMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        /// 3.3.2 Variable header
        Protocol::packString(stream, topic_);
        if (header_.bits.qos > 0) {
            Protocol::packWord(stream, msgId_);
        }

        /// 3.3.3 Payload
        stream.write((const char *) buf_.data(), buf_.size());
    }

    void PublishMessage::decode(boost::asio::streambuf &buf) {
        std::istream stream(&buf);

        /// 3.3.2 Variable header
        topic_ = Protocol::unPackString(stream);
        if (header_.bits.qos > 0) {
            msgId_ = Protocol::unPackWord(stream);
        }

        /// 3.3.3 Payload
        size_t pos = stream.tellg();
        size_t size = buf.size() - pos;

        buf_.resize(size);

        std::memcpy(buf_.data(), (const char *) buf.data().data() + pos, size);
    }

    PubAckMessage::PubAckMessage() : Message(mqtt::MQTT_MSH_PUBACK) {

    }

    void PubAckMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        /// 3.4.2 Variable header
        if (header_.bits.qos > 0) {
            Protocol::packWord(stream, msgId_);
        }
    }

    void PubAckMessage::decode(boost::asio::streambuf &buf) {
        std::istream stream(&buf);

        /// 3.4.2 Variable header
        if (header_.bits.qos > 0) {
            msgId_ = Protocol::unPackWord(stream);
        }
    }

    PubRecMessage::PubRecMessage() : Message(mqtt::MQTT_MSH_PUBREC) {

    }

    void PubRecMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        /// 3.5.2 Variable header
        if (header_.bits.qos > 0) {
            Protocol::packWord(stream, msgId_);
        }
    }

    void PubRecMessage::decode(boost::asio::streambuf &buf) {
        std::istream stream(&buf);

        /// 3.5.2 Variable header
        if (header_.bits.qos > 0) {
            msgId_ = Protocol::unPackWord(stream);
        }
    }

    PubRelMessage::PubRelMessage() : Message(mqtt::MQTT_MSH_PUBREL) {

    }

    void PubRelMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        /// 3.6.2 Variable header
        if (header_.bits.qos > 0) {
            Protocol::packWord(stream, msgId_);
        }
    }

    void PubRelMessage::decode(boost::asio::streambuf &buf) {
        std::istream stream(&buf);

        /// 3.6.2 Variable header
        if (header_.bits.qos > 0) {
            msgId_ = Protocol::unPackWord(stream);
        }
    }

    PubCompMessage::PubCompMessage() : Message(mqtt::MQTT_MSH_PUBCOMP) {

    }

    void PubCompMessage::encode(boost::asio::streambuf &buf) {
        std::ostream stream(&buf);

        /// 3.7.2 Variable header
        if (header_.bits.qos > 0) {
            Protocol::packWord(stream, msgId_);
        }
    }

    void PubCompMessage::decode(boost::asio::streambuf &buf) {
        std::istream stream(&buf);

        /// 3.7.2 Variable header
        if (header_.bits.qos > 0) {
            msgId_ = Protocol::unPackWord(stream);
        }
    }
}
