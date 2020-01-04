//
// Created by Ivan Kishchenko on 2019-07-13.
//

#include "HttpWorker.h"
#include "rpc/json/Spec.h"

#include <boost/beast/version.hpp>
#include <iostream>

using namespace boost;

// Return a reasonable mime type based on the extension of a file.
beast::string_view mimeType(beast::string_view path) {
    using beast::iequals;
    auto const ext = [&path] {
        auto const pos = path.rfind(".");
        if (beast::string_view::npos == pos) {
            return beast::string_view{};
        }

        return path.substr(pos);
    }();
    if (iequals(ext, ".htm")) return "text/html";
    if (iequals(ext, ".html")) return "text/html";
    if (iequals(ext, ".php")) return "text/html";
    if (iequals(ext, ".css")) return "text/css";
    if (iequals(ext, ".txt")) return "text/plain";
    if (iequals(ext, ".js")) return "application/javascript";
    if (iequals(ext, ".json")) return "application/json";
    if (iequals(ext, ".xml")) return "application/xml";
    if (iequals(ext, ".swf")) return "application/x-shockwave-flash";
    if (iequals(ext, ".flv")) return "video/x-flv";
    if (iequals(ext, ".png")) return "image/png";
    if (iequals(ext, ".jpe")) return "image/jpeg";
    if (iequals(ext, ".jpeg")) return "image/jpeg";
    if (iequals(ext, ".jpg")) return "image/jpeg";
    if (iequals(ext, ".gif")) return "image/gif";
    if (iequals(ext, ".bmp")) return "image/bmp";
    if (iequals(ext, ".ico")) return "image/vnd.microsoft.icon";
    if (iequals(ext, ".tiff")) return "image/tiff";
    if (iequals(ext, ".tif")) return "image/tiff";
    if (iequals(ext, ".svg")) return "image/svg+xml";
    if (iequals(ext, ".svgz")) return "image/svg+xml";

    return "application/text";
}

HttpWorker::HttpWorker(JsonRpcHandler::Ptr rpcHandler, boost::asio::io_service& service, tcp::acceptor &acceptor, std::string docRoot)
        : _rpcHandler(rpcHandler)
        , _acceptor(acceptor)
        , _socket(service)
        , _docRoot(std::move(docRoot))
        , _signals(service)
        , _requestDeadline(service, (std::chrono::steady_clock::time_point::max) ())
{
    _signals.add(SIGINT);
    _signals.add(SIGTERM);
#if defined(SIGQUIT)
    _signals.add(SIGQUIT);
#endif

    _signals.async_wait(
            [this,&service](boost::system::error_code errorCode, int) {
                 _acceptor.close();
                service.stop();
            }
    );

}

void HttpWorker::start() {
    accept();
    checkDeadline();
}

void HttpWorker::sendFile(beast::string_view target) {
    // Request path must be absolute and not contain "..".
    if (target.empty() || target[0] != '/' || target.find("..") != std::string::npos) {
        sendBadResponse(http::status::not_found, "File not found\r\n");
        return;
    }

    std::string full_path = _docRoot;
    full_path.append(target.data(), target.size());

    http::file_body::value_type file;
    beast::error_code ec;
    file.open(full_path.c_str(), beast::file_mode::read, ec);
    if (ec) {
        sendBadResponse(http::status::not_found, "File not found\r\n");
        return;
    }

    _fileResponse.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(_alloc));

    _fileResponse->set(http::field::content_type, mimeType(target.to_string()));
    _fileResponse->set(http::field::server, "Beast");
    _fileResponse->body() = std::move(file);
    _fileResponse->result(http::status::ok);
    _fileResponse->keep_alive(false);
    _fileResponse->prepare_payload();

    _fileSerializer.emplace(*_fileResponse);

    http::async_write(
            _socket,
            *_fileSerializer,
            [this](beast::error_code ec, size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                _fileSerializer.reset();
                _fileResponse.reset();
                accept();
            }
    );
}

void HttpWorker::checkDeadline() {
    // The deadline may have moved, so check it has really passed.
    if (_requestDeadline.expiry() <= std::chrono::steady_clock::now()) {
        // Close socket to cancel any outstanding operation.
        beast::error_code ec;
        _socket.close();

        // Sleep indefinitely until we're given a new deadline.
        _requestDeadline.expires_at(std::chrono::steady_clock::time_point::max());
    }

    _requestDeadline.async_wait([this](beast::error_code) {
        checkDeadline();
    });
}

void HttpWorker::accept() {
    // Clean up any previous connection.
    beast::error_code ec;
    _socket.close(ec);
    _buffer.consume(_buffer.size());

    _acceptor.async_accept(
            _socket,
            [this](beast::error_code ec) {
                if (ec) {
                    accept();
                } else {
                    // Request must be fully processed within 60 seconds.
                    _requestDeadline.expires_after(std::chrono::seconds(60));

                    readRequest();
                }
            });
}


void HttpWorker::readRequest() {
    // On each read the parser needs to be destroyed and
    // recreated. We store it in a optional to
    // achieve that.
    //
    // Arguments passed to the parser constructor are
    // forwarded to the message object. A single argument
    // is forwarded to the body constructor.
    //
    // We construct the dynamic body with a 1MB limit
    // to prevent vulnerability to buffer attacks.
    //
    _parser.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(_alloc));

    http::async_read(
            _socket,
            _buffer,
            *_parser,
            [this](beast::error_code ec, size_t) {
                if (ec) {
                    accept();
                } else {
                    processRequest(_parser->get());
                }
            });
}

void HttpWorker::processRequest(const HttpRequest &req) {
    switch (req.method()) {
        case http::verb::get:
            sendFile(req.target());
            break;
        case http::verb::options:
            if (req.target() == "/rpc") {
                _stringResponse.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(_alloc));

                _stringResponse->result(http::status::ok);
                _stringResponse->keep_alive(false);
                _stringResponse->set(http::field::server, "Beast");
                _stringResponse->set(http::field::content_length, 0);
                _stringResponse->set(http::field::allow, "POST");
                _stringResponse->set(http::field::access_control_allow_origin, "*");
                _stringResponse->set(http::field::access_control_allow_methods, "GET,HEAD,OPTIONS,POST,PUP");
                _stringResponse->set(http::field::access_control_allow_headers, "Origin, X-Requested-With, Content-Type, Accept, Authorization");
                _stringResponse->prepare_payload();

                _stringSerializer.emplace(*_stringResponse);

                http::async_write(
                        _socket,
                        *_stringSerializer,
                        [this](beast::error_code ec, size_t) {
                            _socket.shutdown(tcp::socket::shutdown_send, ec);
                            _stringSerializer.reset();
                            _stringResponse.reset();
                            accept();
                        }
                );
            }
            break;
        case http::verb::post: {
            if (req.target() == "/rpc") {
                this->rpc(req);
                break;
            }
        }

        default:
            // We return responses indicating an error if
            // we do not recognize the request method.
            sendBadResponse(
                    http::status::bad_request,
                    "Invalid request-method '" + req.method_string().to_string() + "'\r\n"
            );
            break;
    }
}

void HttpWorker::sendBadResponse(http::status status, std::string const &error) {
    _stringResponse.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(_alloc));

    _stringResponse->result(status);
    _stringResponse->keep_alive(false);
    _stringResponse->set(http::field::server, "Beast");
    _stringResponse->set(http::field::content_type, "text/plain");
    _stringResponse->body() = error;
    _stringResponse->prepare_payload();

    _stringSerializer.emplace(*_stringResponse);

    http::async_write(
            _socket,
            *_stringSerializer,
            [this](beast::error_code ec, size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                _stringSerializer.reset();
                _stringResponse.reset();
                accept();
            }
    );
}

void HttpWorker::rpc(const HttpRequest &req) {
    _stringResponse.emplace(std::piecewise_construct, std::make_tuple(), std::make_tuple(_alloc));

    _stringResponse->keep_alive(false);
    _stringResponse->set(http::field::server, "Beast");

    if (req.method() != http::verb::post) {
        _stringResponse->result(http::status::bad_request);
        _stringResponse->set(http::field::content_type, "text/plain");
        _stringResponse->body() = "json-rpc support only post requests";
    } else if (req.at(http::field::content_type).find("application/json") == std::string::npos) {
        _stringResponse->result(http::status::bad_request);
        _stringResponse->set(http::field::content_type, "text/plain");
        _stringResponse->body() = "json-rpc support only application/json content-type";
    } else {
        std::shared_ptr<JsonRpcRequest> request = std::make_shared<JsonRpcRequest>();
        std::shared_ptr<JsonRpcResponse> response = std::make_shared<JsonRpcResponse>();
        try {
            JsonDecoder(request).decode(req.body());

            _rpcHandler->handle(*request, *response);

        } catch (std::exception& ex) {
            JsonRcpError error;
            error.code = InternalError;
            error.message = ex.what();
            response->error = error;
        }

        _stringResponse->body() = JsonEncoder(response).encode();
    }

    _stringResponse->prepare_payload();
    _stringSerializer.emplace(*_stringResponse);

    http::async_write(
            _socket,
            *_stringSerializer,
            [this](beast::error_code ec, size_t) {
                _socket.shutdown(tcp::socket::shutdown_send, ec);
                _stringSerializer.reset();
                _stringResponse.reset();
                accept();
            }
    );
}


