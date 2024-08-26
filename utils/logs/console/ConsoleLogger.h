#pragma once

#include "ILogger.h"
#include <mutex>

class ConsoleLogger : public ILogger
{
public:
    ConsoleLogger();
    void setLogLevel(LogLevel level) override;
    void log(LogLevel level, const std::string &message) override;

private:
    std::mutex mutex_;
    LogLevel logLevel_;
};