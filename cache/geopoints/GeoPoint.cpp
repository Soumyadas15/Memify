#include "GeoHash.h"
#include "GeoPoint.h"

uint64_t GeoPoint::calculateGeoHash(float latitude, float longitude)
{
    GeoHashBits hash;
    GeoHashRange lat_range = {90.0, -90.0};
    GeoHashRange lon_range = {180.0, -180.0};

    uint8_t step = 13;

    int result = geohash_encode(lat_range, lon_range, latitude, longitude, step, &hash);

    if (result == 0) return hash.bits;
    else return 0;
}