#include "FileLogger.h"

FileLogger::FileLogger(const std::string &filename) : logLevel_(LogLevel::INFO)
{
    logFile_.open(filename, std::ios::app);
}

FileLogger::~FileLogger()
{
    if (logFile_.is_open())
    {
        logFile_.close();
    }
}

void FileLogger::setLogLevel(LogLevel level)
{
    std::lock_guard<std::mutex> lock(mutex_);
    logLevel_ = level;
}

void FileLogger::log(LogLevel level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (level >= logLevel_ && logFile_.is_open())
    {
        logFile_ << formatLogMessage(level, message);
        logFile_.flush();
    }
}