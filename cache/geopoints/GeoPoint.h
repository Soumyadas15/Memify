#ifndef GEOPROINT_H
#define GEOPROINT_H

#include <string>

/**
 * @struct GeoPoint
 * @brief A structure to represent a geographical point with a name, latitude, and longitude.
 */
struct GeoPoint
{
    std::string name; ///< The name associated with the geographical point.
    double latitude;  ///< The latitude of the geographical point.
    double longitude; ///< The longitude of the geographical point.

    /**
     * @brief Default constructor for GeoPoint.
     */
    GeoPoint() : name(""), latitude(0.0), longitude(0.0) {}

    /**
     * @brief Parameterized constructor for GeoPoint.
     *
     * @param name The name of the geographical point.
     * @param latitude The latitude of the geographical point.
     * @param longitude The longitude of the geographical point.
     */
    GeoPoint(const std::string &name, double latitude, double longitude)
        : name(name), latitude(latitude), longitude(longitude) {}
};

#endif // GEOPROINT_H