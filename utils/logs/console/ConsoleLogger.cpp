#include "ConsoleLogger.h"
#include <iostream>

ConsoleLogger::ConsoleLogger() : logLevel_(LogLevel::INFO) {}

void ConsoleLogger::setLogLevel(LogLevel level)
{
    std::lock_guard<std::mutex> lock(mutex_);
    logLevel_ = level;
}

void ConsoleLogger::log(LogLevel level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (level >= logLevel_)
    {
        std::cout << formatLogMessage(level, message);
    }
}