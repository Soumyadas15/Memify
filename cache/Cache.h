#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <list>
#include <mutex>

#include "ICache.h"
#include "GeoPoint.h"
#include "LoggerManager.h"
#include "FileLogger.h"
#include "RTree.h"

/**
 * @class Cache
 * @brief A thread-safe, size-limited cache implementation with time-to-live (TTL) support.
 * 
 * This class implements the ICache interface and provides a cache system that can store, retrieve, 
 * and delete key-value pairs with associated expiration times. It supports a maximum size to limit 
 * memory usage and uses an LRU (Least Recently Used) strategy to evict entries when the cache is full.
 */
class Cache : public ICache
{
public:
    /**
     * @brief Constructs a Cache object with a specified maximum size.
     * 
     * Initializes a cache with a maximum number of entries that can be stored. If the cache reaches 
     * the maximum size, it will evict the least recently used (LRU) entries to make space for new ones.
     * 
     * @param max_size The maximum number of entries the cache can hold. Defaults to 1000.
     */
    explicit Cache(size_t max_size = 1000);

    /**
     * @brief Destructor for the Cache class.
     * 
     * Ensures that all resources are properly released when the Cache object is destroyed.
     */
    ~Cache() override;

    /**
     * @brief Stores a key-value pair in the cache with a specified duration.
     * 
     * Adds a key-value pair to the cache with a specified time-to-live (TTL). If the cache exceeds its 
     * maximum size, the least recently used item will be evicted. If the key already exists, its value 
     * is updated, and its TTL is reset.
     * 
     * @param key A string representing the key.
     * @param value A string representing the value associated with the key.
     * @param duration The time-to-live for the key-value pair. Specified as a duration of type `std::chrono::seconds`.
     */
    void Set(const std::string &key, 
             const std::string &value, 
             std::chrono::seconds duration
    ) override;

    /**
     * @brief Retrieves the value associated with a specified key.
     * 
     * Tries to find a value associated with the provided key. If the key exists in the cache and has not 
     * expired, the value is stored in the output parameter `value`, and the function returns true. If the 
     * key does not exist or has expired, the function returns false. The access will also move the key to 
     * the front of the usage order list, marking it as most recently used.
     * 
     * @param key A string representing the key to search for in the cache.
     * @param value A reference to a string where the found value will be stored.
     * @return `true` if the key exists and the value is successfully retrieved, otherwise `false`.
     */
    bool Get(const std::string &key, std::string &value) override;

    /**
     * @brief Deletes a key-value pair from the cache.
     * 
     * Removes the key-value pair associated with the specified key from the cache. If the key does not exist, 
     * the function has no effect.
     * 
     * @param key A string representing the key of the key-value pair to delete from the cache.
     */
    void Delete(const std::string &key) override;

private:
    /**
     * @struct CacheItem
     * @brief A structure to hold a cache entry's value and expiration time.
     */
    struct CacheItem
    {
        std::string value;                             ///< The cached value associated with the key.
        std::chrono::steady_clock::time_point expiration; ///< The expiration time point for the cache entry.
    };

    size_t max_size_;  ///< The maximum number of entries the cache can hold.
    std::unordered_map<std::string, std::pair<CacheItem, std::list<std::string>::iterator>> items_; ///< A hash map to store cache items and iterators to their positions in the usage order list.
    std::list<std::string> usage_order_; ///< A list to keep track of the usage order of keys, implementing LRU eviction.
    std::mutex mutex_; ///< A mutex to ensure thread-safe operations on the cache.
    std::shared_ptr<FileLogger> file_logger_; ///< A file logger to log activities.

    /**
     * @brief Cleans up expired cache entries.
     * 
     * Iterates through the cache and removes any entries that have expired. This function ensures that the cache 
     * does not hold onto expired data unnecessarily.
     */
    void Cleanup();

    /**
     * @brief Moves a key to the front of the usage order list.
     * 
     * Updates the usage order to mark a key as the most recently used. This is part of the LRU eviction strategy.
     * 
     * @param key The key to move to the front of the usage order.
     */
    void MoveToFront(const std::string &key);

    /**
     * @brief Evicts the least recently used item from the cache.
     * 
     * This function removes the least recently used item from the cache when the cache reaches its maximum size.
     * It ensures that the cache can make space for new entries while adhering to the LRU policy.
     */
    void Evict();
};

#endif // CACHE_H