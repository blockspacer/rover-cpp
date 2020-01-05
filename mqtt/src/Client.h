//
// Created by Ivan Kishchenko on 24.11.2019.
//

#ifndef ROVER_CLIENT_H
#define ROVER_CLIENT_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <protocol/Packet.h>

class Event {
public:
    virtual void onEvent(std::string_view topic) = 0;
};

class Client {
    enum Status {
        Idle,
        Connection,
        Connected
    };
private:
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;

    boost::asio::deadline_timer pingTimer_;

    Status status_;

    uint16_t msgId_ = 0;

private:
    void close() {
        // Cancel all outstanding asynchronous operations.
        socket_.close();

        status_ = Idle;
    }

    void restartPingTimer();

    uint16_t msgId() {
        return ++msgId_;
    }

public:
    Client(boost::asio::io_service &io_service, const std::string &server, int port, const std::string &path);

    void
    handleResolve(const boost::system::error_code &err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    void
    handleConnect(const boost::system::error_code &err, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

    void handleWrite(const boost::system::error_code &err);

    void handleRead(const boost::system::error_code &err);

    void handleTimeOut(const boost::system::error_code &err);

    void subscribe(const std::string &topic);

public:
    void onConnected();
};

#endif //ROVER_CLIENT_H
