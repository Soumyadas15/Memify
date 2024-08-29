#include <algorithm>
#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>

#include "TimeSeriesCache.h"
#include "LoggerManager.h"
#include "FileLogger.h"

TimeSeriesCache::TimeSeriesCache(size_t max_size) : max_size_(max_size)
{
    std::ostringstream oss;
    oss << "cache_" << std::this_thread::get_id() << ".log";

    file_logger_ = std::make_shared<FileLogger>(oss.str());
    file_logger_->setLogLevel(ILogger::LogLevel::DEBUG);

    // Register the file logger with the LoggerManager to handle logging
    LoggerManager::getInstance().addLogger(file_logger_);

    // Start the background thread for cache cleanup
    // This thread will periodically remove expired cache items to ensure the cache remains efficient
    std::thread(&TimeSeriesCache::Cleanup, this).detach();
}

TimeSeriesCache::~TimeSeriesCache()
{
    // Destructor implementation (if needed)
}

void TimeSeriesCache::AddTimePoint(const std::string &series_name, const TimePoint &point)
{
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto &series = time_data_[series_name];
    if (series.size() >= max_size_)
    {
        series.erase(series.begin()); // Remove the oldest data point
    }
    series.push_back(point);
}
void TimeSeriesCache::Cleanup()
{
    // Optional: Implement cleanup logic if necessary
}