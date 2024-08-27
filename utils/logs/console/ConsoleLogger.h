#pragma once

#include "ILogger.h"
#include <mutex>

/**
 * @brief ConsoleLogger is an implementation of the ILogger interface that logs messages to the console.
 *
 * This class provides a way to log messages with different severity levels to the standard output (console).
 * It includes thread safety to ensure that logs are written consistently even when multiple threads are involved.
 * The log level can be set to filter the messages that are logged.
 */
class ConsoleLogger : public ILogger
{
public:
    /**
     * @brief Constructs a ConsoleLogger object with the default log level.
     *
     * The default log level is set to a predefined level (e.g., INFO). This constructor initializes the
     * internal log level and sets up the mutex for thread-safe logging.
     */
    ConsoleLogger();

    /**
     * @brief Sets the log level for the logger.
     *
     * @param level The new log level to be set. Messages with a severity lower than this level will be ignored.
     *
     * This method updates the internal log level. It is useful for filtering out messages based on their severity.
     */
    void setLogLevel(LogLevel level) override;

    /**
     * @brief Logs a message with the specified log level.
     *
     * @param level The severity level of the message. This should be a value from the LogLevel enum.
     * @param message The message to be logged. It will be output to the console.
     *
     * This method logs the provided message to the console if its severity level is greater than or equal
     * to the currently set log level. The logging operation is protected by a mutex to ensure thread safety.
     */
    void log(LogLevel level, const std::string &message) override;

private:
    std::mutex mutex_;  ///< Mutex to ensure thread-safe logging.
    LogLevel logLevel_; ///< The current log level for filtering messages.
};