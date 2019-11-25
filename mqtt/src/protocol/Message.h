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
        Header header_{};
    public:
        virtual void encode(boost::asio::streambuf& buf) = 0;
        virtual void decode(boost::asio::streambuf& buf) = 0;
    };

    class ConnectMessage : public Message {
    private:
        Flags flags_;
    public:
        ConnectMessage();

        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
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

    public:
        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    };


    class PingReqMessage : public Message {
    public:
        PingReqMessage();

        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    };

    class PingRespMessage : public Message {
    public:
        PingRespMessage();

        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    };

    class SubscribeMessage : public Message {
    private:
        std::string topic_;
        uint16_t  msgId_;
    public:
        SubscribeMessage(uint16_t  msgId, const std::string& topic);

        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    };

    class SubsAckMessage : public Message {
    public:
        SubsAckMessage();

        void encode(boost::asio::streambuf& buf) override;
        void decode(boost::asio::streambuf& buf) override;
    };
}


#endif //ROVER_MESSAGE_H
