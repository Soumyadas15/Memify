#include "Cache.h"

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
void Cache::SetGeoPoint(const std::string &key, const GeoPoint &point)
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
            // If the point exists, prepare to remove it from the R-tree
            double min[2] = {name_it->second.longitude, name_it->second.latitude}; // Create the minimum bounding box
            double max[2] = {name_it->second.longitude, name_it->second.latitude}; // Create the maximum bounding box
            rtree_.Remove(min, max, key + ":" + point.name);                       // Remove the existing point from the R-tree
        }
        else
        {
            // If the key does not exist, create a new map for it
            geo_items_[key] = std::unordered_map<std::string, GeoPoint>();
        }
    }

    // Prepare to insert the new point into the R-tree
    double min[2] = {point.longitude, point.latitude}; // Create the minimum bounding box for the new point
    double max[2] = {point.longitude, point.latitude}; // Create the maximum bounding box for the new point
    rtree_.Insert(min, max, key);                      // Insert the new point into the R-tree
    geo_items_[key][point.name] = point;               // Update the cache with the new point

    // Log the operation for auditing and debugging purposes
    file_logger_->info("Set GeoPoint: " + key + " (Name: " + point.name +
                       ", Latitude: " + std::to_string(point.latitude) +
                       ", Longitude: " + std::to_string(point.longitude) + ")");
}