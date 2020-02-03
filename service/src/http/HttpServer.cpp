#include <utility>

//
// Created by Ivan Kishchenko on 2019-07-11.
//

#include "HttpServer.h"
#include "HttpWorker.h"

#include "rpc/HealthRpcSupplier.h"

#include <iostream>

using namespace std;
using namespace boost;

#include "JsonRpcHandler.h"

HttpServer::HttpServer(std::string_view docRoot) : _docRoot(docRoot) {

}

int HttpServer::run(RpcMethod::PtrVec &methods, HttpServerConfig config) {
    JsonRpcHandler::Ptr rpcHandler = std::make_shared<JsonRpcHandler>();

    for (const auto &method : methods) {
        rpcHandler->addMethod(method);
    }

    try {
        auto const address = boost::asio::ip::make_address(config.address.front());
        unsigned short port = config.port.get_value_or(8080);
        int num_workers = config.workers.get_value_or(4);

        asio::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};

        asio::signal_set signals(ioc);
        signals.add(SIGINT);
        signals.add(SIGTERM);
#if defined(SIGQUIT)
        signals.add(SIGQUIT);
#endif
        signals.async_wait(
                [&acceptor, &ioc](boost::system::error_code errorCode, int code) {
                    acceptor.close();
                    ioc.stop();
                });

        std::list<HttpWorker> workers;
        for (int i = 0; i < num_workers; ++i) {
            workers.emplace_back(rpcHandler, ioc, acceptor, _docRoot);
            workers.back().start();
        }

        ioc.run();

        return EXIT_SUCCESS;
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}