#ifndef ITIMESERIESCACHE_H
#define ITIMESERIESCACHE_H

#include <string>
#include <vector>
#include "TimePoint.h"

/**
 * @interface ITimeSeriesCache
 * @brief Interface for a cache managing time series data points.
 */
class ITimeSeriesCache
{
public:
    /**
     * @brief Virtual destructor for the ITimeSeriesCache interface.
     */
    virtual ~ITimeSeriesCache() {}

    /**
     * @brief Adds a time series data point to the cache.
     *
     * @param series_name The name of the time series.
     * @param point The time series data point to add.
     */
    virtual void AddTimePoint(const std::string &series_name, const TimePoint &point) = 0;


    /**
     * @brief Cleans up old time series data based.
     */
    virtual void Cleanup() = 0;
};

#endif // ITIMESERIESCACHE_H