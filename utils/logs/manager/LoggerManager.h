#pragma once

#include "ILogger.h"
#include <memory>
#include <vector>

/**
 * @brief LoggerManager is a singleton class that manages multiple logger instances and provides a unified logging interface.
 *
 * This class allows you to add multiple `ILogger` instances (such as `ConsoleLogger` or `FileLogger`) and log messages
 * with different severity levels. It provides convenience methods for logging messages with specific levels and ensures
 * that all registered loggers receive the log messages.
 */
class LoggerManager
{
public:
    /**
     * @brief Gets the singleton instance of the LoggerManager.
     *
     * @return The singleton instance of LoggerManager.
     *
     * This method returns the single instance of the `LoggerManager` class, creating it if necessary. It ensures that
     * only one instance of `LoggerManager` is used throughout the application.
     */
    static LoggerManager &getInstance();

    /**
     * @brief Adds a logger to the LoggerManager.
     *
     * @param logger A shared pointer to an ILogger instance to be added.
     *
     * This method registers a new logger with the `LoggerManager`. The `LoggerManager` will use this logger to output
     * log messages. Multiple loggers can be added, and they will all receive the log messages.
     */
    void addLogger(std::shared_ptr<ILogger> logger);

    /**
     * @brief Sets the log level for all registered loggers.
     *
     * @param level The new log level to be set. Messages with a severity lower than this level will be ignored.
     *
     * This method updates the log level for all registered loggers, allowing them to filter messages based on severity.
     */
    void setLogLevel(ILogger::LogLevel level);

    /**
     * @brief Logs a message with the specified log level to all registered loggers.
     *
     * @param level The severity level of the message. This should be a value from the LogLevel enum.
     * @param message The message to be logged. It will be passed to all registered loggers.
     *
     * This method iterates over all registered loggers and logs the message if its severity level is greater than or
     * equal to the log level of each logger.
     */
    void log(ILogger::LogLevel level, const std::string &message);

    /**
     * @brief Logs a debug message to all registered loggers.
     *
     * @param message The debug message to be logged. It will be passed to all registered loggers.
     *
     * This method calls the `log` method with the DEBUG level. The message is passed to all registered loggers.
     */
    void debug(const std::string &message);

    /**
     * @brief Logs an info message to all registered loggers.
     *
     * @param message The info message to be logged. It will be passed to all registered loggers.
     *
     * This method calls the `log` method with the INFO level. The message is passed to all registered loggers.
     */
    void info(const std::string &message);

    /**
     * @brief Logs a warning message to all registered loggers.
     *
     * @param message The warning message to be logged. It will be passed to all registered loggers.
     *
     * This method calls the `log` method with the WARNING level. The message is passed to all registered loggers.
     */
    void warning(const std::string &message);

    /**
     * @brief Logs an error message to all registered loggers.
     *
     * @param message The error message to be logged. It will be passed to all registered loggers.
     *
     * This method calls the `log` method with the ERROR level. The message is passed to all registered loggers.
     */
    void error(const std::string &message);

    /**
     * @brief Logs a critical message to all registered loggers.
     *
     * @param message The critical message to be logged. It will be passed to all registered loggers.
     *
     * This method calls the `log` method with the CRITICAL level. The message is passed to all registered loggers.
     */
    void critical(const std::string &message);

private:
    LoggerManager() = default;                                ///< Private default constructor for singleton pattern.
    LoggerManager(const LoggerManager &) = delete;            ///< Delete copy constructor for singleton pattern.
    LoggerManager &operator=(const LoggerManager &) = delete; ///< Delete assignment operator for singleton pattern.

    std::vector<std::shared_ptr<ILogger>> loggers_; ///< List of registered loggers.
};