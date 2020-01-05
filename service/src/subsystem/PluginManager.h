//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#ifndef ROVER_PLUGINGMANAGER_H
#define ROVER_PLUGINGMANAGER_H

#include "SubsystemBase.h"
#include "plugin/Module.h"

#include <rpc/RpcRegistry.h>

#include <map>
#include <vector>

#include <string>

#include <boost/function.hpp>

class PluginManager : public SubsystemBase, public RpcRegistry {
public:
    const char *name() const override;

    void postConstruct(Application &app) override;

    Module::Ptr getModule(const char *name) {
        return getModule(std::string(name));
    }

    Module::Ptr getModule(const std::string &name) {
        Module::Ptr result;
        auto it = _modules.find(name);
        if (it != _modules.end()) {
            result = it->second;
        }

        return result;
    }

    Module::PtrVec getModules() {
        Module::PtrVec result;

        for (auto &_module : _modules) {
            result.push_back(_module.second);
        }

        return result;
    }

    void preDestroy() override;

private:
    std::vector<boost::function<PluginCreator>> _libs;

    std::map<std::string, Module::Ptr> _modules;
};


#endif //ROVER_PLUGINGMANAGER_H
