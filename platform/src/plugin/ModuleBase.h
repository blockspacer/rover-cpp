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
    std::string message(std::string_view msg);

public:
    explicit ModuleBase(Logger::Ptr logger);

    std::map<std::string, RpcMethod::Ptr> getRpcMethods() const override;


    void trace(std::string_view message) override;

    void debug(std::string_view message) override;

    void info(std::string_view message) override;

    void warning(std::string_view message) override;

    void error(std::string_view message) override;

    void fatal(std::string_view message) override;
};


#endif //ROVER_MODULEBASE_H
