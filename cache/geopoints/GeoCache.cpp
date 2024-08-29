#include <algorithm>
#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>

#include "GeoCache.h"
#include "LoggerManager.h"
#include "FileLogger.h"

GeoCache::GeoCache(size_t max_size) : max_size_(max_size)
{
    std::ostringstream oss;
    oss << "cache_" << std::this_thread::get_id() << ".log";

    file_logger_ = std::make_shared<FileLogger>(oss.str());
    file_logger_->setLogLevel(ILogger::LogLevel::DEBUG);

    // Register the file logger with the LoggerManager to handle logging
    LoggerManager::getInstance().addLogger(file_logger_);
    // Start the background thread for cache cleanup
    // This thread will periodically remove expired cache items to ensure the cache remains efficient
}

GeoCache::~GeoCache()
{
    file_logger_->info("Cache destroyed");
    std::cout << "Cache destroyed" << std::endl;
}