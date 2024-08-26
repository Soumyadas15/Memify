#pragma once

#include "ILogger.h"
#include <fstream>
#include <mutex>

class FileLogger : public ILogger {
public:
    FileLogger(const std::string& filename);
    ~FileLogger();
    void setLogLevel(LogLevel level) override;
    void log(LogLevel level, const std::string& message) override;

private:
    std::mutex mutex_;
    std::ofstream logFile_;
    LogLevel logLevel_;
};