//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include <cstddef>
#include "SysInfo.h"

#include <limits>

#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>
#include <cstdio>

#if CFG_OS_MACOSX

#include "platform-mac-os/CpuInfo.h"

#endif

template<typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

// static
int SysInfo::NumberOfProcessors() {
#if CFG_OS_POSIX
    long result = sysconf(_SC_NPROCESSORS_ONLN);  // NOLINT(runtime/int)
    if (result == -1) {
        return 1;
    }
    return static_cast<int>(result);
#endif
}

// static
double SysInfo::cpuTemperature() {
#if CFG_OS_MACOSX
    return SMCGetTemperature(SMC_KEY_CPU_TEMP);
#elif CFG_OS_LINUX
    float millideg;
    FILE *thermal = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    int n = fscanf(thermal, "%f", &millideg);
    fclose(thermal);
    return millideg / 1000;
#else
    return 0;
#endif
}

double SysInfo::batteryTemperature() {
#if CFG_OS_MACOSX
    return SMCGetTemperature(SMC_KEY_BATTERY_TEMP);
#elif CFG_OS_LINUX
    return 0;
#endif
}

// static
int64_t SysInfo::AmountOfPhysicalMemory() {
#if CFG_OS_MACOSX
    int mib[2] = {CTL_HW, HW_MEMSIZE};
    int64_t memsize = 0;
    size_t len = sizeof(memsize);
    if (sysctl(mib, arraysize(mib), &memsize, &len, nullptr, 0) != 0) {
        return 0;
    }
    return memsize;
#elif CFG_OS_POSIX
    long pages = sysconf(_SC_PHYS_PAGES);    // NOLINT(runtime/int)
    long page_size = sysconf(_SC_PAGESIZE);  // NOLINT(runtime/int)
    if (pages == -1 || page_size == -1) {
        return 0;
    }
    return static_cast<int64_t>(pages) * page_size;
#endif
}
