#include <iostream>
#include <thread>
#include <sstream>

#include "Cache.h"
#include "LoggerManager.h"
#include "FileLogger.h"

/**
 * @brief Constructs a Cache object with a specified maximum size.
 *
 * Initializes the LRU (Least Recently Used) cache with a maximum capacity defined by `max_size`. The constructor also
 * sets up logging using `FileLogger` and starts a background thread responsible for periodically cleaning up expired
 * cache items. The background cleanup thread runs indefinitely to ensure that the cache remains free of stale data
 * and operates efficiently.
 *
 * The cache maintains a limit on the number of items it can store. When the cache reaches this limit, it automatically
 * evicts the least recently used items to make room for new entries. This behavior ensures that the cache always contains
 * the most recently accessed data, providing efficient access and memory usage.
 *
 * @param max_size The maximum number of items that the cache can hold. Once this limit is reached, the cache will
 *                 evict the least recently used items to accommodate new entries. The `max_size` should be set according
 *                 to the application's requirements to balance between memory usage and cache performance.
 */
Cache::Cache(size_t max_size) : max_size_(max_size)
{
    // Initialize the file logger with a unique log file name based on the current thread ID
    std::ostringstream oss;
    oss << "cache_" << std::this_thread::get_id() << ".log";

    file_logger_ = std::make_shared<FileLogger>(oss.str());
    file_logger_->setLogLevel(ILogger::LogLevel::DEBUG);

    // Register the file logger with the LoggerManager to handle logging
    LoggerManager::getInstance().addLogger(file_logger_);

    // Start the background thread for cache cleanup
    // This thread will periodically remove expired cache items to ensure the cache remains efficient
    std::thread(&Cache::Cleanup, this).detach();
}

/**
 * @brief Destroys the Cache object and cleans up resources.
 *
 * The destructor logs a message indicating the destruction of the cache. While the destructor does not explicitly stop
 * the background cleanup thread, it ensures that the cache's lifecycle is properly logged. The cleanup thread will
 * continue to run until the application terminates, and it will handle the removal of expired items from the cache.
 *
 * @note The background cleanup thread runs indefinitely. It is essential to manage the thread's lifecycle carefully to
 *       avoid resource leaks or undefined behavior. Proper application shutdown procedures should ensure that all threads
 *       are joined or otherwise handled before the application exits.
 */
Cache::~Cache()
{
    // Log the destruction of the cache
    file_logger_->info("Cache destroyed");
    std::cout << "Cache destroyed" << std::endl;
}