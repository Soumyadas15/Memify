#ifndef ICACHE_H
#define ICACHE_H

#include <string>
#include <chrono>

#include "GeoPoint.h"

/**
 * @class ICache
 * @brief The interface for a Memify cache.
 *
 * This class provides a standard interface for a cache that can store, retrieve, and delete
 * key-value pairs. The values are stored as strings, and each key-value pair is associated
 * with a time-to-live (TTL) duration, specifying how long the pair should remain in the cache.
 */
class ICache
{
public:
    /**
     * @brief Virtual destructor for the ICache interface.
     *
     * A virtual destructor ensures that the derived class's destructor is called
     * when an object is deleted through a base class pointer. This is crucial for
     * properly releasing resources in derived classes.
     */
    virtual ~ICache() = default;

    /**
     * @brief Stores a key-value pair in the cache with a specified duration.
     *
     * This function adds a key-value pair to the cache and associates it with a
     * time-to-live (TTL) duration. After the duration has expired, the key-value
     * pair may be evicted from the cache.
     *
     * @param key A string representing the key.
     * @param value A string representing the value associated with the key.
     * @param duration The time-to-live for the key-value pair. Specified as a duration
     *                 of type `std::chrono::seconds`.
     */
    virtual void Set(const std::string &key, 
                     const std::string &value, 
                     std::chrono::seconds duration) = 0;

    /**
     * @brief Retrieves the value associated with a specified key.
     *
     * This function attempts to find a value associated with the provided key. If the key
     * exists in the cache and has not expired, the value is stored in the output parameter `value`,
     * and the function returns true. If the key does not exist or has expired, the function
     * returns false.
     *
     * @param key A string representing the key to search for in the cache.
     * @param value A reference to a string where the found value will be stored.
     * @return `true` if the key exists and the value is successfully retrieved, otherwise `false`.
     */
    virtual bool Get(const std::string &key, std::string &value) = 0;

    /**
     * @brief Deletes a key-value pair from the cache.
     *
     * This function removes the key-value pair associated with the specified key from the cache.
     * If the key does not exist, the function has no effect.
     *
     * @param key A string representing the key of the key-value pair to delete from the cache.
     */
    virtual void Delete(const std::string &key) = 0;
};

#endif // ICACHE_H