//
// Created by Ivan Kishchenko on 03.02.2020.
//

#ifndef ROVER_PLUGINMANAGERCONFIG_H
#define ROVER_PLUGINMANAGERCONFIG_H

#include <string>
#include "subsystem/config/Config.h"

class PluginManagerConfig : public Config {
public:
    std::string path;
};


#endif //ROVER_PLUGINMANAGERCONFIG_H
