#include <iostream>
#include <thread>

#include "GeoCache.h"

/**
 * @brief Evicts the least recently used (LRU) item from the cache.
 *
 * This method identifies the least recently used item based on the usage order list and removes it from both
 * the cache and the usage order list. This ensures that the cache size stays within the defined limit, allowing
 * new items to be added without exceeding memory constraints.
 *
 * @note This method is called when the cache reaches its maximum capacity. It is crucial for implementing
 *       the LRU eviction policy.
 */
void GeoCache::Evict()
{
    // Check if there are any items to evict
    if (!usage_order_.empty())
    {
        // Identify the least recently used key (last in the usage list)
        auto lru_key = usage_order_.back();

        // Remove the LRU key from the usage list
        usage_order_.pop_back();

        // Remove the LRU key from the cache
        geo_items_.erase(lru_key);

        // Log the eviction of the least recently used item
        file_logger_->info("Evicted least recently used item from Geo cache: '" + lru_key + "'");
        std::cout << "Evicted least recently used item from Geo cache: '" << lru_key << "'" << std::endl;
    }
}