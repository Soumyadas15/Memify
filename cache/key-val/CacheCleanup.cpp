#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <map>
#include <vector>
#include <memory>
#include <sstream>

#include "Cache.h"
#include "LoggerManager.h"
#include "FileLogger.h"

/**
 * @brief Periodically cleans up expired items from the cache.
 *
 * This method runs in an infinite loop, periodically waking up every minute to check and remove expired cache items.
 * It helps to maintain the cache's size and ensures that stale data is not served to clients. The cleanup operation
 * involves locking the cache's mutex to ensure thread safety while modifying the internal data structures.
 *
 * The method performs the following steps:
 * 1. Sleeps for a predefined interval (1 minute) to reduce the frequency of cleanup operations.
 * 2. Acquires a lock on the mutex to ensure that no other thread is modifying the cache while this operation is ongoing.
 * 3. Iterates over the list of cache items and checks each item's expiration time against the current time.
 * 4. Removes items that are expired from both the internal storage and the usage order list.
 * 5. Logs the start and end of the cleanup process, as well as each removal of an expired item.
 *
 * @note This method is designed to run continuously in a separate thread, which is started during the cache's
 *       construction. It does not return any value and runs indefinitely until the application is terminated.
 *
 * @details
 * The cleanup thread operates independently of other threads and ensures that expired items are removed without
 * blocking cache operations. It uses a `std::mutex` to synchronize access to the cache data structures, ensuring
 * that concurrent modifications by other threads do not result in data races or inconsistencies.
 *
 * The logging mechanism captures detailed information about the cleanup process:
 * - **Start of Cleanup Process**: Indicates the beginning of a new cleanup cycle.
 * - **Expired Item Removal**: Logs each removal of an expired item from the cache.
 * - **End of Cleanup Process**: Indicates the completion of the cleanup cycle.
 *
 * Ensure that the cache's destructor and the thread management are handled correctly to avoid resource leaks or
 * undefined behavior. The cleanup thread will continue running even after the `Cache` object is destroyed, so it is
 * crucial to manage the thread's lifetime appropriately.
 */
void Cache::Cleanup()
{
    while (true)
    {
        // Sleep for a defined cleanup interval (1 minute)
        std::this_thread::sleep_for(std::chrono::minutes(1));

        // Lock the mutex to ensure thread-safety while modifying cache data
        std::lock_guard<std::mutex> lock(mutex_);

        // Get the current time to compare with expiration times
        auto now = std::chrono::steady_clock::now();

        // Log the start of the cleanup process

        // Iterate over the cache items
        for (auto it = items_.begin(); it != items_.end();)
        {
            // Check if the current item is expired
            if (it->second.first.expiration <= now)
            {
                // Remove the expired item from the usage list and cache
                usage_order_.erase(it->second.second);
                it = items_.erase(it);

                // Log the removal of an expired item
                file_logger_->info("Expired item removed from cache");
                std::cout << "Expired item removed from cache" << std::endl;
            }
            else
            {
                ++it; // Move to the next item if the current one is not expired
            }
        }
    }
}