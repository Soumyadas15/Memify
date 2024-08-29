#ifndef GEOPROINT_H
#define GEOPROINT_H

#include <string>
#include <cstdint> // For uint64_t

/**
 * @struct GeoPoint
 * @brief A structure to represent a geographical point with a name, latitude, longitude, and GeoHash integer.
 */
struct GeoPoint
{
    std::string name;   ///< The name associated with the geographical point.
    double latitude;    ///< The latitude of the geographical point.
    double longitude;   ///< The longitude of the geographical point.
    double elevation;   ///< The elevation of the geographical point.
    uint64_t geoHash;   ///< The GeoHash integer representation of the geographical point.

    /**
     * @brief Default constructor for GeoPoint.
     */
    GeoPoint() : name(""), latitude(0.0), longitude(0.0), elevation(0.0), geoHash(0) {}

    /**
     * @brief Parameterized constructor for GeoPoint.
     *
     * @param name The name of the geographical point.
     * @param latitude The latitude of the geographical point.
     * @param longitude The longitude of the geographical point.
     * @param elevation The elevation of the geographical point.
     */
    GeoPoint(const std::string &name, double latitude, double longitude, double elevation)
        : name(name), latitude(latitude), longitude(longitude), elevation(elevation)
    {
        geoHash = calculateGeoHash(latitude, longitude);
    }

    /**
     * @brief Calculates the GeoHash integer for the given latitude and longitude.
     *
     * @param latitude The latitude of the geographical point.
     * @param longitude The longitude of the geographical point.
     * @return The GeoHash as a 64-bit integer.
     */
    uint64_t calculateGeoHash(double latitude, double longitude);
};

#endif // GEOPROINT_H