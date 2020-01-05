//
// Created by Ivan Kishchenko on 2019-07-11.
//

#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H


#include <rpc/RpcMethod.h>

class HttpServer {
private:
    std::string _docRoot;
public:
    explicit HttpServer(std::string_view docRoot);

    int run(RpcMethod::PtrVec &methods);
};


#endif //HTTP_SERVER_H
