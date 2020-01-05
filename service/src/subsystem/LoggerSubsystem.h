//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_LOGGERSUBSYSTEM_H
#define ROVER_LOGGERSUBSYSTEM_H


#include "Subsystem.h"
#include <Logger.h>

#include <string>

#define PROP_LOG_LEVEL "log.level"
#define PROP_LOG_ENABLE_FILE "log.file"
#define PROP_LOG_ENABLE_CONSOLE "log.console"

class LoggerSubsystem : public Subsystem, public Logger {
public:
    LoggerSubsystem();

    const char *name() const override;

    void postConstruct(Application &app) override;

    void preDestroy() override;

    void trace(const std::string &message) override;

    void debug(const std::string &message) override;

    void info(const std::string &message) override;

    void warning(const std::string &message) override;

    void error(const std::string &message) override;

    void fatal(const std::string &message) override;
};


#endif //ROVER_LOGGERSUBSYSTEM_H
