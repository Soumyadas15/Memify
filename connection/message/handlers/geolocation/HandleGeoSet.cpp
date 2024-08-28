#include "MessageProcessor.h"
#include <iostream>
#include <stdexcept>

void MessageProcessor::HandleGeoSet(const MESPObject &obj, std::string &response)
{
    // Validate that the command has 5 elements (GEOSET, key, value, latitude, longitude)
    if (obj.arrayValue.size() != 6)
    {
        HandleInvalidCommandFormat(response);
        return;
    }

    // Extract key, value, latitude, and longitude from the command
    const MESPObject &keyObj = obj.arrayValue[1];
    const MESPObject &valueObj = obj.arrayValue[2];
    const MESPObject &latitudeObj = obj.arrayValue[3];
    const MESPObject &longitudeObj = obj.arrayValue[4];
    const MESPObject &elevationObj = obj.arrayValue[5];

    // Check if key and value are of type BulkString
    if (keyObj.type == MESPType::BulkString && valueObj.type == MESPType::BulkString)
    {
        std::string key = keyObj.stringValue;
        std::string value = valueObj.stringValue;

        // Check if latitude and longitude are of type Float
        if (latitudeObj.type == MESPType::Float && longitudeObj.type == MESPType::Float)
        {
            double latitude = latitudeObj.floatValue;
            double longitude = longitudeObj.floatValue;
            double elevation = elevationObj.floatValue;

            // Create GeoPoint object
            GeoPoint geoPoint(value, latitude, longitude, elevation);

            // Update the geo cache with the GeoPoint
            geo_cache_->SetGeoPoint(key, geoPoint);
            response = "OK";
        }
        else
        {
            response = "INVALID GEOPOINT FORMAT";
        }
    }
    else
    {
        HandleInvalidCommandFormat(response);
    }
}