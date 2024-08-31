#ifndef GEO_CACHE_H
#define GEO_CACHE_H

#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
#include <chrono>
#include <thread>
#include <list>

#include "GeoPoint.h"
#include "RTree.h"
#include "LoggerManager.h"
#include "FileLogger.h"
#include "IGeoCache.h"

/**
 * @class GeoCache
 * @brief A cache for managing geo-spatial data.
 */
class GeoCache : public IGeoCache
{
public:
    /**
     * @brief Constructs a GeoCache object with a specified maximum size.
     *
     * @param max_size The maximum number of geo-spatial entries the cache can hold.
     */
    explicit GeoCache(size_t max_size = 1000);

    /**
     * @brief Destructor for the GeoCache class.
     */
    ~GeoCache() override;

    /**
     * @brief Adds a geo-spatial data point to the cache.
     *
     * @param key The key for the geo-spatial entry.
     * @param point The geo-spatial data point to add.
     */
    void SetGeoPoint(const std::string &key, const GeoPoint &point) override;

    /**
     * @brief Retrieves a geo-spatial data point from the cache.
     *
     * @param key The key for the geo-spatial entry.
     * @param name The name associated with the geo-spatial entry.
     * @param point A reference to a GeoPoint object to populate with the retrieved data.
     * @return True if the geo-spatial data point was found and retrieved successfully, false otherwise.
     */
    bool GetGeoPoint(const std::string &key, const std::string &name, GeoPoint &point) override;

    /**
     * @brief Calculates the distance between two geo-spatial points, accounting for both
     *        surface distance on the Earth's sphere and elevation difference.
     *
     * @param point1 The first geo-spatial point
     * @param point2 The second geo-spatial point
     */
    double GetGeoDistance(const GeoPoint &point1, const GeoPoint &point2) override;

private : 
    size_t max_size_;
    std::unordered_map<std::string, std::unordered_map<std::string, GeoPoint>> geo_items_;
    std::list<std::string> usage_order_; ///< A list to keep track of the usage order of keys, implementing LRU eviction.
    typedef RTree<std::string, float, 2, float> RTreeType;
    RTreeType rtree_;
    std::mutex mutex_; ///< A mutex to ensure thread-safe operations on the cache.
    std::shared_ptr<FileLogger> file_logger_;

    /**
     * @brief Evicts the least recently used item from the cache.
     *
     * This function removes the least recently used item from the cache when the cache reaches its maximum size.
     * It ensures that the cache can make space for new entries while adhering to the LRU policy.
     */
    void Evict();
};

#endif // GEO_CACHE_H