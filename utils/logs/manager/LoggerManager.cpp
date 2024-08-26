#include "LoggerManager.h"

    LoggerManager &
    LoggerManager::getInstance()
{
    static LoggerManager instance;
    return instance;
}

void LoggerManager::addLogger(std::shared_ptr<ILogger> logger)
{
    loggers_.push_back(std::move(logger));
}

void LoggerManager::setLogLevel(ILogger::LogLevel level)
{
    for (auto &logger : loggers_)
    {
        logger->setLogLevel(level);
    }
}

void LoggerManager::log(ILogger::LogLevel level, const std::string &message)
{
    for (auto &logger : loggers_)
    {
        logger->log(level, message);
    }
}

void LoggerManager::debug(const std::string &message) { log(ILogger::LogLevel::DEBUG, message); }
void LoggerManager::info(const std::string &message) { log(ILogger::LogLevel::INFO, message); }
void LoggerManager::warning(const std::string &message) { log(ILogger::LogLevel::WARNING, message); }
void LoggerManager::error(const std::string &message) { log(ILogger::LogLevel::ERROR, message); }
void LoggerManager::critical(const std::string &message) { log(ILogger::LogLevel::CRITICAL, message); }