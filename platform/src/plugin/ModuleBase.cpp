//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-15.
//

#include "ModuleBase.h"
#include <sstream>

ModuleBase::ModuleBase(Logger::Ptr logger) : _logger(logger) {
    assert(logger);
}

std::map<std::string, RpcMethod::Ptr> ModuleBase::getRpcMethods() const {
    return getMethods();
}

std::string ModuleBase::message(std::string_view msg) {
    std::stringstream str;
    str << "[mod][" << name() << "] " << msg;

    return str.str();
}

void ModuleBase::trace(std::string_view msg) {
    if (_logger) {
        _logger->trace(message(msg));
    }
}

void ModuleBase::debug(std::string_view msg) {
    if (_logger) {
        _logger->debug(message(msg));
    }
}

void ModuleBase::info(std::string_view msg) {
    if (_logger) {
        _logger->info(message(msg));
    }
}

void ModuleBase::warning(std::string_view msg) {
    if (_logger) {
        _logger->warning(message(msg));
    }
}

void ModuleBase::error(std::string_view msg) {
    if (_logger) {
        _logger->error(message(msg));
    }
}

void ModuleBase::fatal(std::string_view msg) {
    if (_logger) {
        _logger->fatal(message(msg));
    }
}
