#include "Cache.h"
#include <thread>
#include <chrono>
#include <iostream>


Cache::Cache(size_t max_size) : max_size_(max_size)
{
    std::cout << "LRU Cache initialized with max size: " << max_size_ << std::endl;
    std::thread(&Cache::Cleanup, this).detach();
}

Cache::~Cache()
{
    std::cout << "Cache destroyed" << std::endl;
}

void Cache::Set(const std::string &key, const std::string &value, std::chrono::seconds duration)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = items_.find(key);

    if (it != items_.end())
    {
        it->second.first.value = value;
        it->second.first.expiration = std::chrono::steady_clock::now() + duration;
        MoveToFront(key);
    }
    else
    {
        CacheItem item;
        item.value = value;
        item.expiration = std::chrono::steady_clock::now() + duration;
        if (items_.size() >= max_size_)
        {
            Evict();
        }
        auto list_it = usage_order_.insert(usage_order_.begin(), key);
        items_[key] = {item, list_it};
        std::cout << "Key '" << key << "' is SET" << std::endl;
    }
}

bool Cache::Get(const std::string &key, std::string &value)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = items_.find(key);
    if (it != items_.end())
    {
        if (it->second.first.expiration > std::chrono::steady_clock::now())
        {
            value = it->second.first.value;
            MoveToFront(key);
            std::cout << "GET key: '" << key << "' found" << std::endl;
            return true;
        }
        else
        {
            items_.erase(it);
        }
    }
    return false;
}

void Cache::Delete(const std::string &key)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = items_.find(key);
    if (it != items_.end())
    {
        usage_order_.erase(it->second.second);
        items_.erase(it);
        std::cout << "Delete key: '" << key << "', succeeded" << std::endl;
    }
    else
    {
        std::cout << "Delete key: '" << key << "', failed" << std::endl;
    }
}

void Cache::Cleanup()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::minutes(1));
        std::lock_guard<std::mutex> lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        for (auto it = items_.begin(); it != items_.end();)
        {
            if (it->second.first.expiration <= now)
            {
                usage_order_.erase(it->second.second);
                it = items_.erase(it);
                std::cout << "Expired item removed" << std::endl;
            }
            else
            {
                ++it;
            }
        }
    }
}

void Cache::MoveToFront(const std::string &key)
{
    auto it = items_.find(key);
    if (it != items_.end())
    {
        usage_order_.erase(it->second.second);
        it->second.second = usage_order_.insert(usage_order_.begin(), key);
    }
}

void Cache::Evict()
{
    if (!usage_order_.empty())
    {
        auto lru_key = usage_order_.back();
        usage_order_.pop_back();
        items_.erase(lru_key);
        std::cout << "Evicted least recently used item: '" << lru_key << "'" << std::endl;
    }
}