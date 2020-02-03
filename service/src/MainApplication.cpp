//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-18.
//

#include "MainApplication.h"
#include "http/HttpServer.h"
#include "const.h"
#include "subsystem/PluginManager.h"
#include "serial/SerialManager.h"

void MainApplication::postConstruct(Application &app) {
    app.addSubsystem(std::make_shared<SerialManager>());

    Application::postConstruct(app);
}

void MainApplication::run() {
    RpcMethod::PtrVec methods;
    /*
    auto& manager = getSubsystem<PluginManager>();
    for (const auto& module : manager.getModules()) {
        for (const auto& rpc : module->getRpcMethods()) {
            methods.push_back(rpc.second);
        }
    }
     */

    for (const auto &subSystem : this->getSubsystems()) {
        const auto registry = dynamic_cast<RpcRegistry *>(subSystem.get());
        if (registry != nullptr) {
            for (const auto &rpc : registry->getMethods()) {
                logger().info("register rpc-method: " + rpc.first);
                methods.push_back(rpc.second);
            }
        }
    }


    HttpServer server(getProperties()->getString(PROP_APPLICATION_DATA_DIR) + "www");
    server.run(methods, getConfigSource()->get<HttpServerConfig>("http"));
}

int MainApplication::run(int argc, char **argv) {
    return Application::run(argc, argv);
}

void MainApplication::preDestroy() {
    Application::preDestroy();
}
