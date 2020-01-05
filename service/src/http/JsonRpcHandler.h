//
// Created by Ivan Kishchenko on 2019-07-14.
//

#ifndef ROVER_JSONRPCHANDLER_H
#define ROVER_JSONRPCHANDLER_H


#include <rpc/json/Spec.h>
#include <rpc/RpcRegistry.h>

class JsonRpcHandler : public RpcRegistry {
public:
    typedef std::shared_ptr<JsonRpcHandler> Ptr;

    void handle(JsonRpcRequest &request, JsonRpcResponse &response) {
        try {
            response.id = request.id;
            response.jsonrpc = request.jsonrpc;

            const auto method = findMethod(request.method);

            if (method) {
                response.result = method.get()->handle(request.params);
            } else {
                JsonRcpError error;
                error.code = MethodNotFound;
                error.message = "Method not found";
                response.error = error;
            }

        } catch (std::exception &ex) {
            response.jsonrpc = "2.0";
            JsonRcpError error;
            error.code = InternalError;
            error.message = ex.what();
            response.error = error;
        }
    }
};


#endif //ROVER_JSONRPCHANDLER_H
