#include "Cache.h"
#include <iostream>

#include "LoggerManager.h"
#include "FileLogger.h"

/**
 * @brief Sets a key-value pair in the cache with a specified time-to-live (TTL).
 *
 * This method inserts or updates a key-value pair in the cache. If the key already exists, the value is updated,
 * and the key is moved to the front of the usage order list to indicate it has been recently used.
 * If the key does not exist and the cache has reached its maximum size, the least recently used (LRU) item
 * is evicted before inserting the new key-value pair. The item will expire after the specified duration.
 *
 * @param key The key to be set in the cache.
 * @param value The value associated with the key to be set in the cache.
 * @param duration The duration (in seconds) for which the key-value pair should remain in the cache.
 *
 * @note This method is thread-safe and uses a mutex to protect shared resources.
 */
void Cache::Set(const std::string &key, 
                const std::string &value, 
                std::chrono::seconds duration
)
{
    // Lock the mutex to ensure thread-safety
    std::lock_guard<std::mutex> lock(mutex_);

    // Search for the key in the cache
    auto it = items_.find(key);

    // If the key already exists, update the value and expiration time
    if (it != items_.end())
    {
        // Update the value associated with the key
        it->second.first.value = value;
        // Update the expiration time of the key-value pair
        it->second.first.expiration = std::chrono::steady_clock::now() + duration;
        // Move the key to the front of the usage order list
        MoveToFront(key);
    }
    else
    {
        // Key does not exist, create a new CacheItem
        CacheItem item;
        item.value = value;
        item.expiration = std::chrono::steady_clock::now() + duration;

        // Check if the cache has reached its maximum size
        if (items_.size() >= max_size_)
        {
            // Evict the least recently used item
            Evict();
        }

        // Insert the new key at the beginning of the usage order list
        auto list_it = usage_order_.insert(usage_order_.begin(), key);
        // Add the new key-value pair to the cache
        items_[key] = {item, list_it};

        // Log a message indicating the key has been set
        file_logger_->info("Key '" + key + "' is SET");
        std::cout << "Key '" << key << "' is SET" << std::endl;
    }
}