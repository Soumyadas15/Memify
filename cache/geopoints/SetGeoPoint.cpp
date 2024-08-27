#include "Cache.h"

void Cache::SetGeoPoint(const std::string &key, const GeoPoint &point) {
    std::lock_guard<std::mutex> lock(mutex_);
    geo_items_[key] = point;

    file_logger_->info("Set GeoPoint: " + key + " (Name: " + point.name + ", Latitude: " + std::to_string(point.latitude) + ", Longitude: " + std::to_string(point.longitude) + ")");
}