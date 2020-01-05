//
// Created by Ivan Kishchenko on 2019-08-04.
//

#include "SubsystemBase.h"
#include "Application.h"
#include <sstream>

std::string SubsystemBase::message(const std::string &msg) {
    std::stringstream str;
    str << "[sub][" << name() << "] " << msg;

    return str.str();
}

void SubsystemBase::trace(const std::string &msg) {
    if (_logger) {
        _logger->trace(message(msg));
    }
}

void SubsystemBase::debug(const std::string &msg) {
    if (_logger) {
        _logger->debug(message(msg));
    }
}

void SubsystemBase::info(const std::string &msg) {
    if (_logger) {
        _logger->info(message(msg));
    }
}

void SubsystemBase::warning(const std::string &msg) {
    if (_logger) {
        _logger->warning(message(msg));
    }
}

void SubsystemBase::error(const std::string &msg) {
    if (_logger) {
        _logger->error(message(msg));
    }
}

void SubsystemBase::fatal(const std::string &msg) {
    if (_logger) {
        _logger->fatal(message(msg));
    }
}

void SubsystemBase::postConstruct(Application &app) {
    _logger = app.loggerPtr();
}

void SubsystemBase::preDestroy() {

}
