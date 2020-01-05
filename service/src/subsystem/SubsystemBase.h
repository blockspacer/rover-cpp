//
// Created by Ivan Kishchenko on 2019-08-04.
//

#ifndef ROVER_SUBSYSTEMBASE_H
#define ROVER_SUBSYSTEMBASE_H


#include <Logger.h>
#include "Subsystem.h"

class SubsystemBase : public Subsystem, public Logger {
private:
    Logger::Ptr _logger;
private:
    std::string message(const std::string &msg);

public:
    void trace(const std::string &message) override;

    void debug(const std::string &message) override;

    void info(const std::string &message) override;

    void warning(const std::string &message) override;

    void error(const std::string &message) override;

    void fatal(const std::string &message) override;

    void postConstruct(Application &app) override;

    void preDestroy() override;

    ~SubsystemBase() override = default;
};


#endif //ROVER_SUBSYSTEMBASE_H
