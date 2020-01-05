//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#ifndef ROVER_MODULEBASE_H
#define ROVER_MODULEBASE_H


#include <rpc/RpcRegistry.h>
#include "Module.h"

class ModuleBase : public Module, protected RpcRegistry {
private:
    Logger::Ptr _logger;
protected:
    std::string message(const std::string &msg);

public:
    explicit ModuleBase(Logger::Ptr logger);

    std::map<std::string, RpcMethod::Ptr> getRpcMethods() const override;


    void trace(const std::string &message) override;

    void debug(const std::string &message) override;

    void info(const std::string &message) override;

    void warning(const std::string &message) override;

    void error(const std::string &message) override;

    void fatal(const std::string &message) override;
};


#endif //ROVER_MODULEBASE_H
