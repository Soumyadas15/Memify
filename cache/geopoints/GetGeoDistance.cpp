#include "GeoCache.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>

/**
 * @brief Calculates the distance between two geographical points, accounting for both
 *        surface distance on the Earth's sphere and elevation difference.
 *
 * This function uses the Haversine formula to compute the surface distance between
 * two points on the Earth, then adjusts this distance based on the elevation difference
 * between the points using the Pythagorean theorem.
 *
 * The Haversine formula calculates the great-circle distance between two points
 * on the surface of a sphere, given their longitudes and latitudes. The distance
 * is then adjusted to account for the elevation difference.
 *
 * The final distance is computed as the hypotenuse of a right triangle where:
 * - One side is the surface distance calculated by the Haversine formula.
 * - The other side is the difference in elevation between the two points.
 *
 * @param point1 The first geographical point. This point should contain the following fields:
 *               - `latitude`: Latitude of the point in degrees.
 *               - `longitude`: Longitude of the point in degrees.
 *               - `elevation`: Elevation of the point in meters above sea level.
 * @param point2 The second geographical point. This point should have the same structure as `point1`.
 *
 * @return The distance between the two geographical points in meters, considering both
 *         the surface distance and the elevation difference.
 */
double GeoCache::GetGeoDistance(const GeoPoint &point1, const GeoPoint &point2)
{
    // Radius of the Earth in kilometers
    double EARTH_RADIUS = 6371.0;

    // Convert latitude and longitude from degrees to radians
    double lat1 = point1.latitude * M_PI / 180.0;
    double lon1 = point1.longitude * M_PI / 180.0;
    double lat2 = point2.latitude * M_PI / 180.0;
    double lon2 = point2.longitude * M_PI / 180.0;

    // Calculate the differences in latitude and longitude
    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    // Haversine formula to calculate the great-circle distance
    double a = std::sin(dlat / 2) * std::sin(dlat / 2) +
               std::cos(lat1) * std::cos(lat2) *
                   std::sin(dlon / 2) * std::sin(dlon / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    // Surface distance in kilometers
    double surface_distance = EARTH_RADIUS * c;


    // Calculate the elevation difference between the two points
    double elevation_diff = point2.elevation - point1.elevation;
    double res = std::sqrt(surface_distance * surface_distance + elevation_diff * elevation_diff);

    // Pythagorean theorem to account for elevation difference
    return res;
}