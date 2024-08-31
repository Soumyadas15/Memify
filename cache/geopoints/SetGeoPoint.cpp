#include "GeoCache.h"

/**
 * @brief Inserts or updates a geographic point in the cache and the R-tree index.
 *
 * This method adds a new geographic point or updates an existing one in the cache. The point
 * is identified by a unique string key. If a point with the same key already exists, it is
 * removed from the R-tree spatial index before the new point is inserted. The new point is
 * then inserted into both the R-tree index and the `geo_items_` map, which stores all
 * geographic points by key. Additionally, a log message is recorded for each set operation.
 *
 * @param key A unique identifier for the geographic point. It is used to store and access
 *            the point in the cache.
 * @param point A GeoPoint object containing the name, latitude, and longitude of the point.
 */
void GeoCache::SetGeoPoint(const std::string &key, const GeoPoint &point)
{
    // Lock the mutex to ensure thread-safe access to the cache and R-tree
    std::lock_guard<std::mutex> lock(mutex_);

    // Check if a point with the given key already exists in the cache
    auto key_it = geo_items_.find(key);
    if (key_it != geo_items_.end())
    {
        auto name_it = key_it->second.find(point.name);
        if (name_it != key_it->second.end())
        {
            // Prepare to remove the existing point from the R-tree
            float min[3] = {name_it->second.longitude, name_it->second.latitude, name_it->second.elevation};
            float max[3] = {name_it->second.longitude, name_it->second.latitude, name_it->second.elevation};
            rtree_.Remove(min, max, key + ":" + point.name);
        }
    }
    else
    {
        geo_items_[key] = std::unordered_map<std::string, GeoPoint>();
    }

    // Prepare to insert the new point into the R-tree with elevation
    float min[3] = {point.longitude, point.latitude, point.elevation};
    float max[3] = {point.longitude, point.latitude, point.elevation};
    rtree_.Insert(min, max, key + ":" + point.name); // Use 3D bounding box for the new point
    geo_items_[key][point.name] = point;             // Update the cache with the new point

    // Log the operation
    file_logger_->info("Set GeoPoint: " + key + " (Name: " + point.name +
                       ", Latitude: " + std::to_string(point.latitude) +
                       ", Longitude: " + std::to_string(point.longitude) +
                       ", Elevation: " + std::to_string(point.elevation) + ")");
}