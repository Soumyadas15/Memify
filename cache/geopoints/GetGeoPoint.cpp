#include "Cache.h"

bool Cache::GetGeoPoint(const std::string &key, GeoPoint &point) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = geo_items_.find(key);
    if (it != geo_items_.end()) {
        point = it->second;
        return true;
    }
    return false;
}