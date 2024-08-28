#include "GeoCache.h"

// The GetGeoPoint function tries to find a GeoPoint object associated with the given key in the cache.
// If both the outer key and the inner key (name of the GeoPoint) exist, it returns true and outputs
// the GeoPoint object through the reference parameter 'point'. If either key does not exist, it returns false.

// Parameters:
// - key: A constant reference to a std::string representing the unique outer key to look up in the cache.
// - name: A constant reference to a std::string representing the inner key (name of the GeoPoint) to look up in the cache.
// - point: A reference to a GeoPoint object that will store the found value if the keys exist.
//
// Return Value:
// - Returns a boolean value:
//   - true: If both the outer key and the inner key exist in the cache, indicating the GeoPoint object was found.
//   - false: If either key does not exist in the cache, indicating no GeoPoint object was found.
//
// Thread Safety:
// - The function uses a std::lock_guard to acquire a mutex lock before accessing the geo_items_ map.
//   This ensures that the access to the cache is thread-safe, preventing data races and ensuring
//   consistent behavior in a multi-threaded environment.

bool GeoCache::GetGeoPoint(const std::string &key, const std::string &name, GeoPoint &point)
{
    // Acquire a lock to ensure thread safety when accessing the shared resource 'geo_items_'.
    std::lock_guard<std::mutex> lock(mutex_);

    // Attempt to find the outer key in the geo_items_ map.
    auto outer_it = geo_items_.find(key);

    // Check if the outer key was found in the cache.
    if (outer_it != geo_items_.end())
    {
        // Attempt to find the inner key (name) within the map associated with the outer key.
        auto inner_it = outer_it->second.find(name);

        // Check if the inner key was found in the inner map.
        if (inner_it != outer_it->second.end())
        {
            // If both keys exist, copy the GeoPoint object to the 'point' parameter.
            point = inner_it->second;
            file_logger_->info("Found GeoPoint: " + key + " (Name: " + name +
                               ", Latitude: " + std::to_string(point.latitude) +
                               ", Longitude: " + std::to_string(point.longitude) +
                               ", Longitude: " + std::to_string(point.elevation) +
                               ")");
            return true; // Indicate that the GeoPoint object was found.
        }
    }

    // If either key does not exist in the cache, return false to indicate failure.
    return false;
}