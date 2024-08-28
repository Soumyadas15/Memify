#ifndef TIMESERIESCACHE_H
#define TIMESERIESCACHE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <list>
#include <mutex>
#include <chrono>
#include <memory>

#include "ITimeSeriesCache.h"
#include "TimePoint.h"
#include "LoggerManager.h"
#include "FileLogger.h"

/**
 * @class TimeSeriesCache
 * @brief A cache for managing time series data points.
 */
class TimeSeriesCache : public ITimeSeriesCache
{
public:
    /**
     * @brief Constructs a TimeSeriesCache object with a specified maximum size.
     *
     * @param max_size The maximum number of time series entries the cache can hold.
     */
    explicit TimeSeriesCache(size_t max_size = 1000);

    /**
     * @brief Destructor for the TimeSeriesCache class.
     */
    ~TimeSeriesCache();

    /**
     * @brief Adds a time series data point to the cache.
     *
     * @param series_name The name of the time series.
     * @param point The time series data point to add.
     */
    void AddTimePoint(const std::string &series_name, const TimePoint &point) override;

private:
    size_t max_size_; ///< The maximum number of entries the cache can hold.
    std::list<std::string> usage_order_; ///< A list to keep track of the usage order of keys, implementing LRU eviction.
    std::unordered_map<std::string, std::vector<TimePoint>> time_data_;
    std::mutex mutex_; ///< Mutex for thread-safe operations.
    std::shared_ptr<FileLogger> file_logger_;

    /**
     * @brief Cleans up old time series data based on time-to-live (TTL) policy.
     */
    void Cleanup() override;
};

#endif // TIMESERIESCACHE_H