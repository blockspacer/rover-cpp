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

#include "FieldsAlloc.h"
#include "JsonRpcHandler.h"

namespace ip = boost::asio::ip;         // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>
namespace asio = boost::asio;    // from <boost/beast/http.hpp>

typedef http::request<http::string_body, http::basic_fields<fields_alloc<char>>> HttpRequest;
typedef http::response<http::string_body, http::basic_fields<fields_alloc<char>>> HttpResponse;
typedef http::response<http::file_body, http::basic_fields<fields_alloc<char>>> HttpBinaryResponse;

class HttpWorker {
public:
    HttpWorker(HttpWorker const &) = delete;

    HttpWorker &operator=(HttpWorker const &) = delete;

    HttpWorker(JsonRpcHandler::Ptr rpcRegistry, asio::io_service& service, tcp::acceptor &acceptor, std::string docRoot);

    void start();

private:
    boost::asio::signal_set _signals;


    using alloc_t = fields_alloc<char>;
    //using request_body_t = http::basic_dynamic_body<boost::beast::flat_static_buffer<1024 * 1024>>;
    using request_body_t = http::string_body;

    // The acceptor used to listen for incoming connections.
    tcp::acceptor &_acceptor;

    // The path to the root of the document directory.
    std::string _docRoot;

    // The socket for the currently connected client.
    tcp::socket _socket;

    // The buffer for performing reads
    boost::beast::flat_static_buffer<32768> _buffer;

    // The allocator used for the fields in the request and reply.
    alloc_t _alloc{32768};

    // The parser for reading the requests
    boost::optional<http::request_parser<request_body_t, alloc_t>> _parser;

    // The timer putting a time limit on requests.
    boost::asio::basic_waitable_timer<std::chrono::steady_clock> _requestDeadline;

    // The string-based response message.
    boost::optional<HttpResponse> _stringResponse;

    // The string-based response serializer.
    boost::optional<http::response_serializer<http::string_body, http::basic_fields<alloc_t>>> _stringSerializer;

    // The file-based response message.
    boost::optional<HttpBinaryResponse> _fileResponse;

    // The file-based response serializer.
    boost::optional<http::response_serializer<http::file_body, http::basic_fields<alloc_t>>> _fileSerializer;

    const JsonRpcHandler::Ptr _rpcHandler;
private:
    void accept();

    void readRequest();

    void processRequest(const HttpRequest &req);

    void sendBadResponse(http::status status, std::string const &error);

    void sendFile(boost::beast::string_view target);

    void checkDeadline();

    void rpc(const HttpRequest &req);
};

#endif //ROVER_HTTPWORKER_H
