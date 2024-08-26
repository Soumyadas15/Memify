#pragma once

#include "ILogger.h"
#include <memory>
#include <vector>

class LoggerManager {
public:
    static LoggerManager& getInstance();

    void addLogger(std::shared_ptr<ILogger> logger);
    void setLogLevel(ILogger::LogLevel level);
    void log(ILogger::LogLevel level, const std::string& message);

    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);

private:
    LoggerManager() = default;
    LoggerManager(const LoggerManager&) = delete;
    LoggerManager& operator=(const LoggerManager&) = delete;

    std::vector<std::shared_ptr<ILogger>> loggers_;
};