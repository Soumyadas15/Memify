#include "LoggerManager.h"

/**
 * @brief Gets the singleton instance of LoggerManager.
 *
 * @return The singleton instance of LoggerManager.
 *
 * This static method provides access to the single instance of the `LoggerManager` class, creating it on the first
 * call and ensuring that there is only one instance throughout the application. This implementation uses the
 * "lazy initialization" pattern, where the instance is created only when needed.
 */
LoggerManager &LoggerManager::getInstance()
{
    static LoggerManager instance; ///< The singleton instance of LoggerManager.
    return instance;
}

/**
 * @brief Adds a logger to the LoggerManager.
 *
 * @param logger A shared pointer to an ILogger instance to be added.
 *
 * This method adds a new logger to the `LoggerManager`'s collection of loggers. The logger will be used to output
 * log messages according to the configured log level. The `std::move` function is used to transfer ownership of the
 * logger object to the `loggers_` vector.
 */
void LoggerManager::addLogger(std::shared_ptr<ILogger> logger)
{
    loggers_.push_back(std::move(logger)); ///< Adds the logger to the internal collection.
}

/**
 * @brief Sets the log level for all registered loggers.
 *
 * @param level The new log level to be set. Messages with a severity lower than this level will be ignored.
 *
 * This method updates the log level for all loggers managed by the `LoggerManager`. It iterates over each logger in
 * the `loggers_` vector and calls its `setLogLevel` method. This ensures that all loggers filter messages according
 * to the new log level.
 */
void LoggerManager::setLogLevel(ILogger::LogLevel level)
{
    for (auto &logger : loggers_)
    {
        logger->setLogLevel(level); ///< Updates the log level for each logger.
    }
}

/**
 * @brief Logs a message with the specified log level to all registered loggers.
 *
 * @param level The severity level of the message. This should be a value from the LogLevel enum.
 * @param message The message to be logged. It will be passed to all registered loggers.
 *
 * This method iterates over all registered loggers and calls their `log` method with the specified level and message.
 * This ensures that each logger processes the log message according to its configuration.
 */
void LoggerManager::log(ILogger::LogLevel level, const std::string &message)
{
    for (auto &logger : loggers_)
    {
        logger->log(level, message); ///< Sends the log message to each logger.
    }
}

/**
 * @brief Logs a debug message to all registered loggers.
 *
 * @param message The debug message to be logged. It will be passed to all registered loggers.
 *
 * This method calls the `log` method with the DEBUG level. The message is passed to all registered loggers.
 */
void LoggerManager::debug(const std::string &message)
{
    log(ILogger::LogLevel::DEBUG, message); ///< Logs the message at DEBUG level.
}

/**
 * @brief Logs an info message to all registered loggers.
 *
 * @param message The info message to be logged. It will be passed to all registered loggers.
 *
 * This method calls the `log` method with the INFO level. The message is passed to all registered loggers.
 */
void LoggerManager::info(const std::string &message)
{
    log(ILogger::LogLevel::INFO, message); ///< Logs the message at INFO level.
}

/**
 * @brief Logs a warning message to all registered loggers.
 *
 * @param message The warning message to be logged. It will be passed to all registered loggers.
 *
 * This method calls the `log` method with the WARNING level. The message is passed to all registered loggers.
 */
void LoggerManager::warning(const std::string &message)
{
    log(ILogger::LogLevel::WARNING, message); ///< Logs the message at WARNING level.
}

/**
 * @brief Logs an error message to all registered loggers.
 *
 * @param message The error message to be logged. It will be passed to all registered loggers.
 *
 * This method calls the `log` method with the ERROR level. The message is passed to all registered loggers.
 */
void LoggerManager::error(const std::string &message)
{
    log(ILogger::LogLevel::ERROR, message); ///< Logs the message at ERROR level.
}

/**
 * @brief Logs a critical message to all registered loggers.
 *
 * @param message The critical message to be logged. It will be passed to all registered loggers.
 *
 * This method calls the `log` method with the CRITICAL level. The message is passed to all registered loggers.
 */
void LoggerManager::critical(const std::string &message)
{
    log(ILogger::LogLevel::CRITICAL, message); ///< Logs the message at CRITICAL level.
}