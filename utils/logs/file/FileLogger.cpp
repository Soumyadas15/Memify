#include "FileLogger.h"

/**
 * @brief Constructs a FileLogger object with the specified log file.
 *
 * @param filename The name of the file where logs will be written.
 *
 * Opens the specified file for appending log messages. If the file cannot be opened, it will result in an
 * exception, which should be handled by the caller. Initializes the log level to INFO by default and sets up
 * a mutex for thread-safe logging.
 */
FileLogger::FileLogger(const std::string &filename) : logLevel_(LogLevel::INFO)
{
    logFile_.open(filename, std::ios::app);
    if (!logFile_.is_open())
    {
        throw std::runtime_error("Unable to open log file: " + filename);
    }
}

/**
 * @brief Destructs the FileLogger object and closes the log file.
 *
 * Closes the log file to ensure that all buffered log messages are flushed and resources are properly released.
 */
FileLogger::~FileLogger()
{
    if (logFile_.is_open())
    {
        logFile_.close();
    }
}

/**
 * @brief Sets the log level for the logger.
 *
 * @param level The new log level to be set. Messages with a severity lower than this level will be ignored.
 *
 * Updates the internal log level to the specified value. The operation is performed inside a mutex lock to ensure
 * thread safety, preventing race conditions in a multi-threaded environment.
 */
void FileLogger::setLogLevel(LogLevel level)
{
    std::lock_guard<std::mutex> lock(mutex_);
    logLevel_ = level;
}

/**
 * @brief Logs a message with the specified log level.
 *
 * @param level The severity level of the message. This should be a value from the LogLevel enum.
 * @param message The message to be logged. It will be written to the log file.
 *
 * Checks if the provided log level is greater than or equal to the currently set log level. If so, the message
 * is written to the log file and the file is flushed to ensure that the message is immediately written to disk.
 * The logging operation is protected by a mutex to ensure that multiple threads do not interfere with each other's
 * logging operations.
 */
void FileLogger::log(LogLevel level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (level >= logLevel_ && logFile_.is_open())
    {
        logFile_ << formatLogMessage(level, message);
        logFile_.flush();
    }
}