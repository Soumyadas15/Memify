// ILogger.h
#pragma once

#include <string>
#include <chrono>
#include <ctime>

class ILogger
{
public:
    enum class LogLevel
    {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    virtual ~ILogger() = default;
    virtual void setLogLevel(LogLevel level) = 0;
    virtual void log(LogLevel level, const std::string &message) = 0;

    virtual void debug(const std::string &message) { log(LogLevel::DEBUG, message); }
    virtual void info(const std::string &message) { log(LogLevel::INFO, message); }
    virtual void warning(const std::string &message) { log(LogLevel::WARNING, message); }
    virtual void error(const std::string &message) { log(LogLevel::ERROR, message); }
    virtual void critical(const std::string &message) { log(LogLevel::CRITICAL, message); }

protected:
    static std::string formatLogMessage(LogLevel level, const std::string &message)
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        char timeBuffer[26];
        ctime_r(&now_c, timeBuffer);
        timeBuffer[24] = '\0';

        const char *levelStr;
        switch (level)
        {
        case LogLevel::DEBUG:
            levelStr = "DEBUG";
            break;
        case LogLevel::INFO:
            levelStr = "INFO";
            break;
        case LogLevel::WARNING:
            levelStr = "WARNING";
            break;
        case LogLevel::ERROR:
            levelStr = "ERROR";
            break;
        case LogLevel::CRITICAL:
            levelStr = "CRITICAL";
            break;
        }

        return std::string(timeBuffer) + " [" + levelStr + "] " + message + "\n";
    }
};