#include <iostream>

#include "Cache.h"

/**
 * @brief Removes a key-value pair from the cache if the key exists.
 *
 * This method attempts to find a specified key in the cache. If the key is found, it deletes the key-value pair
 * from both the internal data structure and the usage order list, effectively removing the key from the cache.
 * If the key does not exist, no changes are made.
 *
 * @param key The key to be removed from the cache.
 *
 * @note This method is thread-safe and uses a mutex to protect shared resources.
 */
void Cache::Delete(const std::string &key)
{
    // Lock the mutex to ensure thread-safety
    std::lock_guard<std::mutex> lock(mutex_);

    // Attempt to find the key in the cache
    auto it = items_.find(key);

    // Check if the key exists in the cache
    if (it != items_.end())
    {
        // Remove the key from the usage order list
        usage_order_.erase(it->second.second);

        // Remove the key-value pair from the cache
        items_.erase(it);

        // Log a message indicating successful deletion
        file_logger_->info("DELETE key '" + key + "': succeeded");
        std::cout << "DELETE key '" << key << "': succeeded" << std::endl;
    }
    else
    {
        // Log a message indicating the key was not found
        file_logger_->info("DELETE key '" + key + "': failed");
        std::cout << "DELETE key: '" << key << "': failed" << std::endl;
    }
}