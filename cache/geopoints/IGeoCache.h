#ifndef IGEO_CACHE_H
#define IGEO_CACHE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include "GeoPoint.h"

/**
 * @class IGeoCache
 * @brief An interface for a cache managing geo-spatial data.
 */
class IGeoCache
{
public:
    /**
     * @brief Constructs an IGeoCache object. Derived classes should implement this.
     */
    virtual ~IGeoCache() = default;

    /**
     * @brief Adds a geo-spatial data point to the cache.
     *
     * @param key The key for the geo-spatial entry.
     * @param point The geo-spatial data point to add.
     */
    virtual void SetGeoPoint(const std::string &key, const GeoPoint &point) = 0;

    /**
     * @brief Retrieves a geo-spatial data point from the cache.
     *
     * @param key The key for the geo-spatial entry.
     * @param name The name associated with the geo-spatial entry.
     * @param point A reference to a GeoPoint object to populate with the retrieved data.
     * @return True if the geo-spatial data point was found and retrieved successfully, false otherwise.
     */
    virtual bool GetGeoPoint(const std::string &key, const std::string &name, GeoPoint &point) = 0;

    virtual double GetGeoDistance(const GeoPoint &point1, const GeoPoint &point2) = 0;
};

#endif // IGEO_CACHE_H