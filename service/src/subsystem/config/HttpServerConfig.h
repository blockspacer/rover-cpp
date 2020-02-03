//
// Created by Ivan Kishchenko on 03.02.2020.
//

#ifndef ROVER_HTTPSERVERCONFIG_H
#define ROVER_HTTPSERVERCONFIG_H

#include "ConfigHelper.h"
#include <vector>
#include <string>
#include <optional>

BEGIN_DECLARE_CFG(HttpServerConfig)
__DECLARE_CFG_FIELDS3(
        boost::optional<int>, port,
        std::vector<std::string>, address,
        boost::optional<int>, workers
)
END_DECLARE_CFG(HttpServerConfig)

#endif //ROVER_HTTPSERVERCONFIG_H
