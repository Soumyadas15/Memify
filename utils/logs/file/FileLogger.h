#pragma once

#include "ILogger.h"
#include <fstream>
#include <mutex>

/**
 * @brief FileLogger is an implementation of the ILogger interface that logs messages to a file.
 *
 * This class provides a mechanism to log messages with different severity levels to a specified file.
 * It supports thread-safe logging and can filter messages based on the set log level.
 */
class FileLogger : public ILogger
{
public:
    /**
     * @brief Constructs a FileLogger object with the specified log file.
     *
     * @param filename The name of the file where logs will be written.
     *
     * This constructor opens the log file for writing and initializes the log level to a default value (e.g., INFO).
     * It also sets up a mutex for thread-safe logging operations.
     */
    FileLogger(const std::string &filename);

    /**
     * @brief Destructs the FileLogger object and closes the log file.
     *
     * The destructor ensures that the log file is properly closed when the logger object is destroyed.
     */
    ~FileLogger();

    /**
     * @brief Sets the log level for the logger.
     *
     * @param level The new log level to be set. Messages with a severity lower than this level will be ignored.
     *
     * This method updates the internal log level and is thread-safe due to the mutex.
     */
    void setLogLevel(LogLevel level) override;

    /**
     * @brief Logs a message with the specified log level.
     *
     * @param level The severity level of the message. This should be a value from the LogLevel enum.
     * @param message The message to be logged. It will be written to the log file.
     *
     * This method writes the provided message to the log file if its severity level is greater than or equal
     * to the currently set log level. The logging operation is protected by a mutex to ensure that multiple
     * threads do not interfere with each other's logging operations.
     */
    void log(LogLevel level, const std::string &message) override;

private:
    std::mutex mutex_;      ///< Mutex to ensure thread-safe logging.
    std::ofstream logFile_; ///< Output file stream for writing log messages.
    LogLevel logLevel_;     ///< The current log level for filtering messages.
};