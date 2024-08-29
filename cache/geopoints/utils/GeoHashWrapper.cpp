#include "GeoHash.h"
#include <cstdint>

extern "C" {
    uint64_t geohash_int(double latitude, double longitude, unsigned int precision);
}

uint64_t calculate_geohash(double latitude, double longitude) {
    return geohash_int(latitude, longitude, 12);
}