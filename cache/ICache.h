#ifndef ICACHE_H
#define ICACHE_H

#include <string>
#include <chrono>

class ICache
{
public:
    virtual ~ICache() = default;
    virtual void Set(const std::string &key, const std::string &value, std::chrono::seconds duration) = 0;
    virtual bool Get(const std::string &key, std::string &value) = 0;
    virtual void Delete(const std::string &key) = 0;
};

#endif