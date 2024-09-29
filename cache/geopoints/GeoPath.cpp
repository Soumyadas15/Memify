#include "GeoCache.h"

bool GeoCache::GeoPath(const GeoPoint &point1, const GeoPoint &point2)
{
    // Acquire a lock to ensure thread safety when accessing shared resources.
    std::lock_guard<std::mutex> lock(mutex_);

    // Calculate the distance using GetGeoDistance.
    double distance = GetGeoDistance(point1, point2);

    // Add the path to the adjacency list with the GeoPoint data and distance as weight.
    adjList[point1.name].emplace_back(point2.name, point2);
    adjList[point2.name].emplace_back(point1.name, point1);

    file_logger_->info("Route added to adjacency list");

    // Return true indicating success.
    return true;
}
