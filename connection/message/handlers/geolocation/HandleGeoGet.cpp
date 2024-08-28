#include "MessageProcessor.h"
#include <iostream>
#include <stdexcept>

void MessageProcessor::HandleGeoGet(const MESPObject &obj, std::string &response)
{
    // Check if the command contains exactly two elements: the command itself and the key
    if (obj.arrayValue.size() != 3)
    {
        HandleInvalidCommandFormat(response);
        return;
    }

    // Extract the key from the command arguments
    const MESPObject &keyObj = obj.arrayValue[1];
    const MESPObject &nameObj = obj.arrayValue[2];

    // Check if the key is of type BulkString
    if (keyObj.type == MESPType::BulkString && nameObj.type == MESPType::BulkString)
    {
        std::string key = keyObj.stringValue;
        std::string nameKey = nameObj.stringValue;
        std::string name;
        float latitude;
        float longitude;
        float elevation;
        GeoPoint geoPoint(name, latitude, longitude, elevation);

        // Retrieve the value associated with the key from the gep cache
        if (geo_cache_ ->GetGeoPoint(key, nameKey, geoPoint))
        {
            std::cout << std::to_string(geoPoint.elevation) << std::endl;
            // Set the response to the retrieved value
            response = "Name: " + geoPoint.name +
                       ", Latitude: " + std::to_string(geoPoint.latitude) +
                       ", Longitude: " + std::to_string(geoPoint.longitude);
        }
        else
        {
            // Set the response to "NOT FOUND" if the key is not in the cache
            response = "NOT FOUND";
        }
    }
    else
    {
        // Handle invalid command format if the key is not of type BulkString
        HandleInvalidCommandFormat(response);
    }
}