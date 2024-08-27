#include "ConsoleLogger.h"
#include <iostream>

/**
 * @brief Constructs a ConsoleLogger object with the default log level.
 *
 * The default log level is set to INFO. This constructor initializes the internal log level to INFO
 * and sets up the mutex for ensuring thread-safe logging operations.
 */
ConsoleLogger::ConsoleLogger() : logLevel_(LogLevel::INFO) {}

/**
 * @brief Sets the log level for the logger.
 *
 * @param level The new log level to be set. Messages with a severity lower than this level will be ignored.
 *
 * This method updates the internal log level. It is useful for filtering out messages based on their
 * severity. The update operation is performed inside a mutex lock to ensure thread safety, preventing
 * race conditions from multiple threads setting the log level simultaneously.
 */
void ConsoleLogger::setLogLevel(LogLevel level)
{
    std::lock_guard<std::mutex> lock(mutex_);
    logLevel_ = level;
}

/**
 * @brief Logs a message with the specified log level.
 *
 * @param level The severity level of the message. This should be a value from the LogLevel enum.
 * @param message The message to be logged. It will be output to the console.
 *
 * This method checks if the provided log level is greater than or equal to the currently set log level.
 * If so, the message is formatted and output to the console. The logging operation is protected by a
 * mutex to ensure that multiple threads do not interfere with each other's logging operations.
 */
void ConsoleLogger::log(LogLevel level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (level >= logLevel_)
    {
        std::cout << formatLogMessage(level, message);
    }
}