#pragma once

#include <string>
#include <chrono>
#include <ctime>

/**
 * @brief Abstract base class for logging functionality.
 *
 * The ILogger class provides a general interface for logging messages at various levels of severity.
 * Derived classes should implement the specific logging behavior, such as writing to a file or console.
 *
 * This class allows setting the minimum log level and provides methods to log messages at different severity levels:
 * DEBUG, INFO, WARNING, ERROR, and CRITICAL.
 */
class ILogger
{
public:
    /**
     * @brief Enumeration representing different log levels.
     *
     * Log levels are used to categorize the severity of log messages.
     * Messages with a higher severity level will be logged only if the current log level is set to the same level or lower.
     */
    enum class LogLevel
    {
        DEBUG,   /**< Detailed information for diagnosing issues. */
        INFO,    /**< General information about application operations. */
        WARNING, /**< Potentially harmful situations or minor issues. */
        ERROR,   /**< Error events that might cause problems but do not halt execution. */
        CRITICAL /**< Severe error events that will likely cause program termination. */
    };

    /**
     * @brief Destructor.
     *
     * Virtual destructor to ensure that derived class destructors are called correctly.
     */
    virtual ~ILogger() = default;

    /**
     * @brief Sets the minimum log level.
     *
     * This method sets the log level threshold. Messages with a severity level lower than the set level will be ignored.
     *
     * @param level The minimum log level to set.
     */
    virtual void setLogLevel(LogLevel level) = 0;

    /**
     * @brief Logs a message with a specified log level.
     *
     * This method should be implemented by derived classes to handle the actual logging behavior.
     *
     * @param level The severity level of the log message.
     * @param message The log message to be recorded.
     */
    virtual void log(LogLevel level, const std::string &message) = 0;

    /**
     * @brief Logs a DEBUG level message.
     *
     * This method calls the `log` method with LogLevel::DEBUG.
     *
     * @param message The log message to be recorded.
     */
    virtual void debug(const std::string &message) { log(LogLevel::DEBUG, message); }

    /**
     * @brief Logs an INFO level message.
     *
     * This method calls the `log` method with LogLevel::INFO.
     *
     * @param message The log message to be recorded.
     */
    virtual void info(const std::string &message) { log(LogLevel::INFO, message); }

    /**
     * @brief Logs a WARNING level message.
     *
     * This method calls the `log` method with LogLevel::WARNING.
     *
     * @param message The log message to be recorded.
     */
    virtual void warning(const std::string &message) { log(LogLevel::WARNING, message); }

    /**
     * @brief Logs an ERROR level message.
     *
     * This method calls the `log` method with LogLevel::ERROR.
     *
     * @param message The log message to be recorded.
     */
    virtual void error(const std::string &message) { log(LogLevel::ERROR, message); }

    /**
     * @brief Logs a CRITICAL level message.
     *
     * This method calls the `log` method with LogLevel::CRITICAL.
     *
     * @param message The log message to be recorded.
     */
    virtual void critical(const std::string &message) { log(LogLevel::CRITICAL, message); }

protected:
    /**
     * @brief Formats a log message with timestamp and log level.
     *
     * This static method formats the log message with a timestamp and log level label.
     * It is used to ensure a consistent format for log messages.
     *
     * @param level The severity level of the log message.
     * @param message The log message to be formatted.
     * @return A formatted string containing the timestamp, log level, and message.
     */
    static std::string formatLogMessage(LogLevel level, const std::string &message)
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        char timeBuffer[26];
        ctime_r(&now_c, timeBuffer);
        timeBuffer[24] = '\0'; // Remove the newline character added by ctime_r

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