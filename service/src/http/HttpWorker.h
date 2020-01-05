#include <utility>

//
// Created by Ivan Kishchenko on 2019-07-13.
//

#ifndef ROVER_HTTPWORKER_H
#define ROVER_HTTPWORKER_H

#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <rpc/RpcRegistry.h>

#include "JsonRpcHandler.h"

namespace ip = boost::asio::ip;         // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace asio = boost::asio;    // from <boost/beast/http.hpp>

typedef http::request_parser<http::string_body> HttpRequestParser;
typedef http::request<http::string_body> HttpRequest;
typedef http::response<http::string_body> HttpResponse;
typedef http::response<http::file_body> HttpBinaryResponse;
typedef http::response_serializer<http::string_body> HttpSerializer;
typedef http::response_serializer<http::file_body> HttpBinarySerializer;

class HttpWorker {
public:
    HttpWorker(HttpWorker const &) = delete;

    HttpWorker &operator=(HttpWorker const &) = delete;

    HttpWorker(JsonRpcHandler::Ptr rpcRegistry, asio::io_service &service, tcp::acceptor &acceptor,
               std::string_view docRoot);

    void start();

private:
    // The acceptor used to listen for incoming connections.
    tcp::acceptor &_acceptor;

    // The socket for the currently connected client.
    tcp::socket _socket;

    // The path to the root of the document directory.
    std::string _docRoot;

    // The buffer for performing reads
    boost::beast::flat_static_buffer<32768> _buffer;

    // The parser for reading the requests
    boost::optional<HttpRequestParser> _parser;

    // The timer putting a time limit on requests.
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> _requestDeadline;

    // The string-based response message.
    boost::optional<HttpResponse> _response;

    // The string-based response serializer.
    boost::optional<HttpSerializer> _serializer;

    // The file-based response message.
    boost::optional<HttpBinaryResponse> _fileResponse;

    // The file-based response serializer.
    boost::optional<HttpBinarySerializer> _fileSerializer;

    JsonRpcHandler::Ptr _rpcHandler;
private:
    void accept();

    void readRequest();

    void processRequest(const HttpRequest &req);

    void sendBadResponse(http::status status, std::string_view error);

    void sendFile(std::string_view target);

    void checkDeadline();

    void rpc(const HttpRequest &req);
};

#endif //ROVER_HTTPWORKER_H
