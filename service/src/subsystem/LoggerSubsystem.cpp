//
// Created by Ivan Kishchenko (Lazada Group) on 2019-07-12.
//

#include "LoggerSubsystem.h"

#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include "subsystem/Application.h"
#include "const.h"

using namespace boost;

LoggerSubsystem::LoggerSubsystem() = default;

const char *LoggerSubsystem::name() const {
    return "logger";
}

void LoggerSubsystem::postConstruct(Application &app) {
    std::string filePath = app.getProperties()->getString(PROP_APPLICATION_LOG_DIR)
                           + app.getProperties()->getString(PROP_APPLICATION_NAME) + ".log";

    log::register_simple_formatter_factory<log::trivial::severity_level, char>("Severity");

    if (app.getProperties()->getBoolean(PROP_LOG_ENABLE_FILE, true)) {
        log::add_file_log(
                log::keywords::file_name = filePath,
                log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%",
                log::keywords::auto_flush = true
        );
    }

    if (app.getProperties()->getBoolean(PROP_LOG_ENABLE_CONSOLE, true)) {
        log::add_console_log(
                std::cout,
                log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
        );
    }

    auto logLevel = app.getProperties()->getString(PROP_LOG_LEVEL, "debug");
    auto logLevelValue = log::trivial::trace;

    if (logLevel == "trace") {
        logLevelValue = log::trivial::trace;
    } else if (logLevel == "debug") {
        logLevelValue = log::trivial::debug;
    } else if (logLevel == "info") {
        logLevelValue = log::trivial::info;
    } else if (logLevel == "warning") {
        logLevelValue = log::trivial::warning;
    } else if (logLevel == "error") {
        logLevelValue = log::trivial::error;
    } else if (logLevel == "fatal") {
        logLevelValue = log::trivial::fatal;
    }

    log::core::get()->set_filter(log::trivial::severity >= logLevelValue);

    log::add_common_attributes();
}

void LoggerSubsystem::preDestroy() {

}

void LoggerSubsystem::trace(std::string_view message) {
    BOOST_LOG_TRIVIAL(trace) << message;
}

void LoggerSubsystem::debug(std::string_view message) {
    BOOST_LOG_TRIVIAL(debug) << message;
}

void LoggerSubsystem::info(std::string_view message) {
    BOOST_LOG_TRIVIAL(info) << message;
}

void LoggerSubsystem::warning(std::string_view message) {
    BOOST_LOG_TRIVIAL(warning) << message;
}

void LoggerSubsystem::error(std::string_view message) {
    BOOST_LOG_TRIVIAL(error) << message;
}

void LoggerSubsystem::fatal(std::string_view message) {
    BOOST_LOG_TRIVIAL(fatal) << message;
}
