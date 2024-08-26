#ifndef ISNAPSHOT_H
#define ISNAPSHOT_H

#include <unordered_map>
#include <string>
#include <chrono>

class ISnapshot
{
public:
    virtual ~ISnapshot() = default;

    virtual void Save(const std::unordered_map<std::string, std::pair<std::string, std::chrono::steady_clock::time_point>>& data) = 0;
    virtual void Load(std::unordered_map<std::string, std::pair<std::string, std::chrono::steady_clock::time_point>>& data) = 0;
};

#endif