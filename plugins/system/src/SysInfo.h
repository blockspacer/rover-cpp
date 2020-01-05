//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#ifndef ROVER_SYSINFO_H
#define ROVER_SYSINFO_H


#include <cstdint>
#include "config.h"

class SysInfo {
public:
    static int NumberOfProcessors();

    static int64_t AmountOfPhysicalMemory();

    static double cpuTemperature();

    static double batteryTemperature();
};


#endif //ROVER_SYSINFO_H
