#include "Cache.h"

// The GetGeoPoint function tries to find a GeoPoint object associated with the given key in the cache.
// If the key exists, it returns true and outputs the GeoPoint object through the reference parameter 'point'.
// If the key does not exist, it returns false.

// Parameters:
// - key: A constant reference to a std::string representing the unique key to look up in the cache.
// - point: A reference to a GeoPoint object that will store the found value if the key exists.
//
// Return Value:
// - Returns a boolean value:
//   - true: If the key exists in the cache, indicating the GeoPoint object was found.
//   - false: If the key does not exist in the cache, indicating no GeoPoint object was found.
//
// Thread Safety:
// - The function uses a std::lock_guard to acquire a mutex lock before accessing the geo_items_ map.
//   This ensures that the access to the cache is thread-safe, preventing data races and ensuring
//   consistent behavior in a multi-threaded environment.

bool Cache::GetGeoPoint(const std::string &key, GeoPoint &point)
{
    // Acquire a lock to ensure thread safety when accessing the shared resource 'geo_items_'.
    std::lock_guard<std::mutex> lock(mutex_);

    // Attempt to find the GeoPoint object associated with the given key in the geo_items_ map.
    auto it = geo_items_.find(key);

    // Check if the key was found in the cache.
    if (it != geo_items_.end())
    {
        // If the key exists, copy the GeoPoint object to the 'point' parameter.
        point = it->second;
        return true; // Indicate that the key was found and the GeoPoint object is valid.
    }

    // If the key does not exist in the cache, return false to indicate failure.
    return false;
}