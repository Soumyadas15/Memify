#ifndef CACHE_H
#define CACHE_H

#include "ICache.h"


#include <unordered_map>
#include <list>
#include <mutex>

class Cache : public ICache
{
public:
    explicit Cache(size_t max_size = 1000);
    ~Cache() override;
    void Set(const std::string &key, const std::string &value, std::chrono::seconds duration) override;
    bool Get(const std::string &key, std::string &value) override;
    void Delete(const std::string &key) override;

private:
    struct CacheItem
    {
        std::string value;
        std::chrono::steady_clock::time_point expiration;
    };

    size_t max_size_;
    std::unordered_map<std::string, std::pair<CacheItem, std::list<std::string>::iterator>> items_;
    std::list<std::string> usage_order_;
    std::mutex mutex_;

    void Cleanup();
    void MoveToFront(const std::string &key);
    void Evict();
};

#endif