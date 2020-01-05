//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-07.
//

#include <rpc/RpcMonitorSupplier.h>
#include "SystemModule.h"
#include <iostream>

#include "config.h"

#if CFG_OS_MACOSX

#include "platform-mac-os/CpuInfo.h"

#endif

namespace plugin {

    SystemModule::SystemModule(Logger::Ptr logger) : ModuleBase(logger) {
#if CFG_OS_MACOSX
        SMCOpen();
#endif
        info("Create SystemModule");
        addMethod(std::make_shared<RpcMonitorSupplier>());
    }

    std::string SystemModule::name() const {
        return "system";
    }

    SystemModule::~SystemModule() {
        info("Destroy SystemModule");
#if CFG_OS_MACOSX
        SMCClose();
#endif
    }
}