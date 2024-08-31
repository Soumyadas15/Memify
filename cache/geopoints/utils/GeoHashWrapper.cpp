#include "GeoHash.h"
#include <cstdint>

extern "C" {
    uint64_t geohash_int(float latitude, float longitude, unsigned int precision);
}

uint64_t calculate_geohash(float latitude, float longitude) {
    return geohash_int(latitude, longitude, 12);
}