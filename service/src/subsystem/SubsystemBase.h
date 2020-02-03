//
// Created by Ivan Kishchenko on 2019-08-04.
//

#ifndef ROVER_SUBSYSTEMBASE_H
#define ROVER_SUBSYSTEMBASE_H


#include <Logger.h>
#include "Subsystem.h"
#include "Application.h"
#include <sstream>

class SubsystemBase : public Subsystem, public Logger {
private:
    Logger::Ptr _logger;
private:
    std::string message(const std::string_view msg) {
        std::stringstream str;
        str << "[sub][" << name() << "] " << msg;

        return str.str();
    }

public:
    void trace(std::string_view msg) override {
        if (_logger) {
            _logger->trace(message(msg));
        }
    }

    void debug(const std::string_view msg) override {
        if (_logger) {
            _logger->debug(message(msg));
        }
    }

    void info(std::string_view msg) override {
        if (_logger) {
            _logger->info(message(msg));
        }
    }

    void warning(std::string_view msg) override {
        if (_logger) {
            _logger->warning(message(msg));
        }
    }

    void error(std::string_view msg) override {
        if (_logger) {
            _logger->error(message(msg));
        }
    }

    void fatal(std::string_view msg) override {
        if (_logger) {
            _logger->fatal(message(msg));
        }
    }

    void postConstruct(Application &app) override {
        _logger = app.loggerPtr();
    }

    void preDestroy() override {

    }

    ~SubsystemBase() override = default;
};


#endif //ROVER_SUBSYSTEMBASE_H
