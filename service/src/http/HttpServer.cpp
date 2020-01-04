#include <utility>

//
// Created by Ivan Kishchenko on 2019-07-11.
//

#include "HttpServer.h"
#include "HttpWorker.h"

#include "rpc/HealthRpcSupplier.h"

#include <iostream>

using namespace std;

#include "JsonRpcHandler.h"

HttpServer::HttpServer(std::string docRoot) : _docRoot(std::move(docRoot)) {

}

int HttpServer::run(RpcMethod::PtrVec& methods) {
    JsonRpcHandler::Ptr rpcHandler = std::make_shared<JsonRpcHandler>();

    for (const auto& method : methods) {
        rpcHandler->addMethod(method);
    }

    try {
        auto const address = boost::asio::ip::make_address("0.0.0.0");
        unsigned short port = 8080;
        int num_workers = 4;
        //bool spin = (std::strcmp(argv[5], "spin") == 0);

        boost::asio::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};

        std::list<HttpWorker> workers;
        for (int i = 0; i < num_workers; ++i) {
            workers.emplace_back(rpcHandler, ioc, acceptor, _docRoot);
            workers.back().start();
        }

        ioc.run();

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}