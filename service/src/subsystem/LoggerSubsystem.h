//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_LOGGERSUBSYSTEM_H
#define ROVER_LOGGERSUBSYSTEM_H


#include "Subsystem.h"
#include <Logger.h>

#include <string>
#include <subsystem/config/Config.h>

#define PROP_LOG_LEVEL "log.level"
#define PROP_LOG_ENABLE_FILE "log.file"
#define PROP_LOG_ENABLE_CONSOLE "log.console"

class LoggerSubsystem : public Subsystem, public Logger {
public:
    LoggerSubsystem();

    const char *name() const override;

    void postConstruct(Application &app) override;

    void preDestroy() override;

    void trace(std::string_view message) override;

    void debug(std::string_view message) override;

    void info(std::string_view message) override;

    void warning(std::string_view message) override;

    void error(std::string_view message) override;

    void fatal(std::string_view message) override;
};


#endif //ROVER_LOGGERSUBSYSTEM_H
