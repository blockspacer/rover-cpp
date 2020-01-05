//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#ifndef ROVER_LOGGER_H
#define ROVER_LOGGER_H


#include <string>

#define PROP_LOG_LEVEL "log.level"
#define PROP_LOG_ENABLE_FILE "log.file"
#define PROP_LOG_ENABLE_CONSOLE "log.console"

#include <memory>

class Logger {
public:
    typedef std::shared_ptr<Logger> Ptr;

    virtual void trace(const std::string &message) = 0;

    virtual void debug(const std::string &message) = 0;

    virtual void info(const std::string &message) = 0;

    virtual void warning(const std::string &message) = 0;

    virtual void error(const std::string &message) = 0;

    virtual void fatal(const std::string &message) = 0;
};


#endif //ROVER_LOGGER_H
