#include <iostream>
#include <thread>

#include "Cache.h"


/**
 * @brief Moves the specified key to the front of the usage list, indicating recent access.
 *
 * This method updates the usage order of the specified key by moving it to the front of the list.
 * This operation is critical in maintaining the least-recently-used (LRU) cache eviction policy, where
 * frequently accessed items remain in the cache longer.
 *
 * @param key The key to be moved to the front of the usage list.
 *
 * @note This method assumes that the caller already holds the mutex lock to ensure thread-safety.
 */
void Cache::MoveToFront(const std::string &key)
{
    // Find the key in the cache
    auto it = items_.find(key);

    // Check if the key exists
    if (it != items_.end())
    {
        // Remove the current position from the usage order
        usage_order_.erase(it->second.second);

        // Insert the key at the front of the usage list to mark it as recently used
        it->second.second = usage_order_.insert(usage_order_.begin(), key);
    }
}