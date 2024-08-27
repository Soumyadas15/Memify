#include "Cache.h"
#include <iostream>

/**
 * @brief Retrieves the value associated with a given key from the cache, if it exists and is not expired.
 *
 * This method looks up a key in the cache and, if found and not expired, returns the associated value.
 * It also updates the usage order to mark the key as recently used. If the key is expired, it removes the key from the cache.
 *
 * @param key The key to search for in the cache.
 * @param value A reference to a string where the value associated with the key will be stored if found.
 * @return true If the key is found and the value is not expired, false otherwise.
 *
 * @note This method is thread-safe and uses a mutex to protect shared resources.
 */
bool Cache::Get(const std::string &key, std::string &value)
{
    // Lock the mutex to ensure thread-safety
    std::lock_guard<std::mutex> lock(mutex_);

    // Attempt to find the key in the cache
    auto it = items_.find(key);

    // If the key is found in the cache
    if (it != items_.end())
    {
        // Check if the key has not expired
        if (it->second.first.expiration > std::chrono::steady_clock::now())
        {
            // Retrieve the value associated with the key
            value = it->second.first.value;

            // Update the usage order by moving the key to the front
            MoveToFront(key);

            // Log a message indicating the key has been found
            file_logger_->info("GET key '" + key + "': found");
            std::cout << "GET key '" << key << "': found" << std::endl;
            return true;
        }
        else
        {
            // If the key has expired, remove it from the cache
            items_.erase(it);
        }
    }

    // Return false if the key is not found or is expired
    return false;
}