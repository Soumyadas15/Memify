#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

#include "ConnectionHandler.h"
#include "MessageProcessor.h"
#include "HMACUtil.h"
#include "LoggerManager.h"
#include "FileLogger.h"

#include "ICache.h"
#include "IGeoCache.h"
#include "ITimeSeriesCache.h"

/**
 * @class ConnectionHandler
 * @brief Manages communication with a single client over a socket.
 *
 * The `ConnectionHandler` class is responsible for managing a network connection with a client. It handles receiving data from the client, processing messages, verifying message signatures for security, and sending responses back to the client. The class also provides detailed logging for debugging and monitoring purposes.
 */
ConnectionHandler::ConnectionHandler(
    std::shared_ptr<ICache> cache,
    std::shared_ptr<IGeoCache> geo_cache,
    std::shared_ptr<ITimeSeriesCache> time_series_cache,
    int client_fd,
    const std::string &secret_key)
    : cache_(std::move(cache)),
      geo_cache_(std::move(geo_cache)),
      time_series_cache_(std::move(time_series_cache)),
      client_fd_(client_fd),
      secret_key_(secret_key)
{
    // Create a log file name specific to this connection using the client file descriptor.
    std::ostringstream oss;
    oss << "connection_" << client_fd_ << ".log";

    // Initialize a FileLogger to handle logging for this connection.
    file_logger_ = std::make_shared<FileLogger>(oss.str());
    file_logger_->setLogLevel(ILogger::LogLevel::DEBUG);  // Set log level to DEBUG for detailed information.

    // Add the FileLogger instance to the global LoggerManager to manage all logging activities.
    LoggerManager::getInstance().addLogger(file_logger_);

    // Log the initiation of a new connection.
    file_logger_->info("New connection started for client_fd: " + std::to_string(client_fd_));
}
