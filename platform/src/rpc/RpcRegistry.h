//
// Created by Ivan Kishchenko on 2019-06-16.
//

#ifndef ROVER_RPCREGISTRY_H
#define ROVER_RPCREGISTRY_H

#include <map>
#include "RpcMethod.h"

class RpcRegistry {
private:
    std::map<std::string, RpcMethod::Ptr> _methods;

public:
    template<class T>
    void addMethod(const std::shared_ptr<T> method) {
        _methods[method->name()] = method;
    }

    boost::optional<RpcMethod::Ptr> findMethod(const std::string &method) {
        boost::optional<RpcMethod::Ptr> result;
        auto iter = _methods.find(method);
        if (iter != _methods.end()) {
            result = iter->second;
        }

        return result;
    }

    std::map<std::string, RpcMethod::Ptr> getMethods() const {
        return _methods;
    }
};

#endif //ROVER_RPCREGISTRY_H
