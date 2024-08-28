#ifndef TIMESERIESPOINT_H
#define TIMESERIESPOINT_H

#include <string>
#include <vector>

/**
 * @struct TimePoint
 * @brief A structure to represent a time series point with a timestamp and a value.
 */
struct TimePoint
{
    std::string timestamp; ///< The timestamp of the time series point, formatted as a string (e.g., ISO 8601).
    double value;          ///< The value associated with the time series point.

    /**
     * @brief Default constructor for TimeSeriesPoint.
     */
    TimePoint() : timestamp(""), value(0.0) {}

    /**
     * @brief Parameterized constructor for TimeSeriesPoint.
     *
     * @param timestamp The timestamp of the time series point.
     * @param value The value associated with the time series point.
     */
    TimePoint(const std::string &timestamp, double value)
        : timestamp(timestamp), value(value) {}
};

#endif // TIMEPOINT_H