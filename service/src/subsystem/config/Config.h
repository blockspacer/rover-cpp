//
// Created by Ivan Kishchenko on 03.02.2020.
//

#ifndef ROVER_CONFIG_H
#define ROVER_CONFIG_H

#include "ConfigHelper.h"

BEGIN_DECLARE_CFG(LogConfig)
__DECLARE_CFG_FIELDS3(
        std::string, level,
        bool, file,
        bool, console
)
END_DECLARE_CFG(LogConfig)

BEGIN_DECLARE_CFG(Config)
__DECLARE_CFG_FIELDS1(LogConfig, log)
END_DECLARE_CFG(Config)

#endif //ROVER_CONFIG_H
