//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#include "Application.h"
#include <boost/dll.hpp>
#include <properties/PropertiesMap.h>
#include <properties/PropertiesContainer.h>
#include <properties/PropertiesJson.h>
#include <iostream>
#include <memory>
#include "const.h"

#include "subsystem/LoggerSubsystem.h"
#include "subsystem/PluginManager.h"
#include <rpc/RpcRegistry.h>

#include "config/Config.h"
#include "config/HttpServerConfig.h"

using namespace std;

const char *Application::name() const {
    return "main";
}

Logger &Application::logger() {
    return *_logger;
}

Logger::Ptr Application::loggerPtr() {
    return _logger;
}

void Application::postConstruct(Application &app) {
    logger().info("construct application");
    for (auto &spSubsystem : _spSubsystems) {
        logger().info(std::string("construct ") + spSubsystem->name());
        spSubsystem->postConstruct(app);
    }
}

void Application::preDestroy() {
    logger().info("destroy application");
    for (auto iter = _spSubsystems.rbegin(); iter != _spSubsystems.rend(); ++iter) {
        logger().info(std::string("destroy ") + (*iter)->name());
        (*iter)->preDestroy();
    }
}

int Application::run(int argc, char *argv[]) {
    auto properties = std::make_shared<PropertiesContainer>();
    auto appPath = boost::dll::program_location();
    auto path = boost::dll::program_location().parent_path();

    logger().debug("load env properties");
    auto envProperties = std::make_shared<PropertiesMap>();

    envProperties->setString(PROP_APPLICATION_PATH, path.string());
    envProperties->setString(PROP_APPLICATION_LIBRARY, path.parent_path().append("lib/").string());
    envProperties->setString(PROP_APPLICATION_NAME, appPath.filename().string());
    envProperties->setString(PROP_APPLICATION_CONFIG_DIR, path.parent_path().append("etc/").string());
    envProperties->setString(PROP_APPLICATION_DATA_DIR, path.parent_path().append("var/").string());
    envProperties->setString(PROP_APPLICATION_TEMP_DIR, path.parent_path().append("var/tmp/").string());
    envProperties->setString(PROP_APPLICATION_LOG_DIR, path.parent_path().append("var/logs/").string());
    properties->addProperties(envProperties);

    logger().debug("try load application properties");
    try {
        _configSource = std::make_shared<ConfigSource>(path.parent_path().append("etc").append(DEFAULT_CONFIG_NAME).string());
        auto logCfg = _configSource->get<LogConfig>("log");
        auto cfg = _configSource->get<Config>();
        auto httpConfig = _configSource->get<HttpServerConfig>("http");

        auto appProperties = std::make_shared<PropertiesJson>();
        appProperties->load(path.parent_path().append("etc").append(DEFAULT_CONFIG_NAME).string());
        properties->addProperties(appProperties);

    } catch (std::exception &ex) {
        std::cout << "can't load default settings: " << ex.what() << std::endl;
    }

    _properties = properties;

    logger().debug("init log system");
    ((LoggerSubsystem *) (_logger.get()))->postConstruct(*this);

    this->addSubsystem(std::make_shared<PluginManager>());

    postConstruct(*this);

    run();

    preDestroy();

    return 0;
}

void Application::run() {
    logger().info("Application running");
}

Application::Application() {
    _logger = make_shared<LoggerSubsystem>();
}

Application::~Application() = default;

