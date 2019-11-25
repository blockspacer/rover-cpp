//
// Created by Ivan Kishchenko on 24.11.2019.
//

#include <protocol/Packet.h>
#include "Client.h"

#include "protocol/Protocol.h"
#include "protocol/Message.h"
#include <iostream>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

Client::Client(io_service &io_service, const std::string &server, int port, const std::string &path)
        : resolver_(io_service), socket_(io_service), status_(Idle),
          pingTimer_(io_service, boost::posix_time::seconds(5)) {
    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    tcp::resolver::query query(server, std::to_string(port));
    resolver_.async_resolve(
            query,
            boost::bind(
                    &Client::handleResolve, this,
                    asio::placeholders::error,
                    asio::placeholders::iterator
            )
    );
}

void Client::handleResolve(const system::error_code &err, tcp::resolver::iterator endpoint_iterator) {
    if (!err) {
        // Attempt a connection to the first endpoint in the list. Each endpoint
        // will be tried until we successfully establish a connection.
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(
                endpoint,
                boost::bind(
                        &Client::handleConnect,
                        this,
                        boost::asio::placeholders::error,
                        ++endpoint_iterator
                )
        );
    }
}

void Client::handleConnect(const system::error_code &err, tcp::resolver::iterator endpoint_iterator) {
    if (!err) {
        // The connection was successful. Send the request.
        mqtt::ConnectMessage connectRequest{};
        connectRequest.encode(request_);

        status_ = Connection;

        async_write(
                socket_,
                request_,
                boost::bind(
                        &Client::handleWrite,
                        this,
                        boost::asio::placeholders::error
                )
        );
    } else if (endpoint_iterator != tcp::resolver::iterator()) {
        // The connection failed. Try the next endpoint in the list.
        socket_.close();
        tcp::endpoint endpoint = *endpoint_iterator;
        socket_.async_connect(
                endpoint,
                boost::bind(
                        &Client::handleConnect,
                        this,
                        boost::asio::placeholders::error,
                        ++endpoint_iterator
                )
        );
    } else {
    }
}

void Client::handleWrite(const boost::system::error_code &err) {
    if (err.value()) {
        close();
    } else {
        boost::asio::async_read(
                socket_,
                response_,
                boost::asio::transfer_at_least(1),
                boost::bind(
                        &Client::handleRead,
                        this,
                        boost::asio::placeholders::error
                )
        );
    }
}

void Client::handleRead(const boost::system::error_code &err) {
    if (err.value()) {
        close();
    } else {
        try {
            while (response_.size()) {
                mqtt::Header header{};
                header.all = *(uint8_t *) response_.data().data();
                std::cout << header.bits.type << std::endl;
                switch (header.bits.type) {
                    case mqtt::MQTT_MSG_CONNACK: {
                        mqtt::ConnAckMessage ack{};
                        ack.decode(response_);
                        status_ = Connected;
                        onConnected();
                    }
                        break;
                    case mqtt::MQTT_MSH_PINGRESP: {
                        mqtt::PingRespMessage resp{};
                        resp.decode(response_);
                        break;
                    }
                    default: {
                        response_.consume(response_.size());
                        //close();
                    }
                        break;
                }
            }

            restartPingTimer();
        } catch (std::exception &ex) {
            std::cout << ex.what();
        }
    }

}
void Client::restartPingTimer() {
    pingTimer_.expires_at(pingTimer_.expires_at() + boost::posix_time::seconds(5));
    pingTimer_.async_wait([this](const boost::system::error_code &err) { this->handleTimeOut(err);} );
}

void Client::handleTimeOut(const boost::system::error_code &err) {
    std::cout << "Ping timeout" << std::endl;

    mqtt::PingReqMessage pingReqMessage{};
    pingReqMessage.encode(request_);

    async_write(
            socket_,
            request_,
            boost::bind(
                    &Client::handleWrite,
                    this,
                    boost::asio::placeholders::error
            )
    );
}

void Client::subscribe(const std::string& topic) {
    mqtt::SubscribeMessage subscribe(msgId(), topic);
    subscribe.encode(request_);

    async_write(
            socket_,
            request_,
            boost::bind(
                    &Client::handleWrite,
                    this,
                    boost::asio::placeholders::error
            )
    );
}

void Client::onConnected() {
    //subscribe("test");
}