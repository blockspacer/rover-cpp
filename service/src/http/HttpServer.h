//
// Created by Ivan Kishchenko on 2019-07-11.
//

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H


#include <rpc/RpcMethod.h>
#include <subsystem/config/HttpServerConfig.h>

class HttpServer {
private:
    std::string _docRoot;
public:
    explicit HttpServer(std::string_view docRoot);

    int run(RpcMethod::PtrVec &methods, HttpServerConfig config);
};


#endif //HTTP_SERVER_H
