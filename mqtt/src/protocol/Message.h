//
// Created by Ivan Kishchenko on 24.11.2019.
//

#ifndef ROVER_MESSAGE_H
#define ROVER_MESSAGE_H

#include <boost/asio/streambuf.hpp>
#include "Packet.h"

namespace mqtt {

    class Message {
    protected:
        uint8_t _type;
        Header header_{};
    protected:
        virtual void encode(boost::asio::streambuf& buf) { };
        virtual void decode(boost::asio::streambuf& buf) { };
    public:
        explicit Message(unsigned int type);
        virtual void pack(boost::asio::streambuf& buf);
        virtual void unPack(boost::asio::streambuf& buf);
    };

    class ConnectMessage : public Message {
    private:
        Flags flags_{};
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        ConnectMessage();
    };

    class ConnAckMessage : public Message {
        union {
            uint8_t all;	/**< all connack flags */
            struct
            {
                bool sessionPresent : 1;    /**< was a session found on the server? */
                unsigned int reserved : 7;	/**< message type nibble */
            } bits;
        } flags_{};	 /**< connack flags byte */
        uint8_t rc_{}; /**< connack reason code */
        uint16_t version_{};  /**< the version of MQTT */
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        ConnAckMessage();
    };

    class PingReqMessage : public Message {
    public:
        PingReqMessage();
    };

    class PingRespMessage : public Message {
    public:
        PingRespMessage();
    };

    class SubscribeMessage : public Message {
    private:
        uint16_t  msgId_;
        std::string topic_;
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        SubscribeMessage(uint16_t  msgId, std::string_view topic);
    };

    class SubsAckMessage : public Message {
    private:
        uint16_t  msgId_{};
        uint8_t rc_{};
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        SubsAckMessage();
    };

    class UnSubscribeMessage : public Message {
    private:
        uint16_t  msgId_{};
        std::string topic_;
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        UnSubscribeMessage(uint16_t  msgId, std::string_view topic);
    };

    class UnSubAckMessage : public Message {
    private:
        uint16_t  msgId_{};
        uint8_t rc_{};
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        UnSubAckMessage();
    };

    class DisconnectMessage : public Message {
    public:
        DisconnectMessage();
    };

    class PublishMessage : public Message {
    private:
        uint16_t  msgId_{};
        std::string topic_;
        std::vector<std::byte> buf_;
    public:
        PublishMessage(uint16_t  msgId, std::string_view topic, const std::byte* data, std::size_t size);
        PublishMessage(uint16_t  msgId, std::string_view topic);
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    };

    class PubAckMessage : public Message {
    private:
        uint16_t  msgId_{};
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        PubAckMessage();
    };

    class PubRecMessage : public Message {
    private:
        uint16_t  msgId_{};
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        PubRecMessage();
    };

    class PubRelMessage : public Message {
    private:
        uint16_t  msgId_{};
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        PubRelMessage();
    };

    class PubCompMessage : public Message {
    private:
        uint16_t  msgId_{};
    protected:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    public:
        PubCompMessage();
    };
}


#endif //ROVER_MESSAGE_H
