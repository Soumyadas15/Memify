#include "MessageProcessor.h"
#include <iostream>
#include <stdexcept>


void MessageProcessor::HandleGeoPath(const MESPObject &obj, std::string &response)
{
    // Check if the command contains exactly four elements
    if (obj.arrayValue.size() != 4)
    {
        HandleInvalidCommandFormat(response);
        return;
    }

    // Extract the key and location names from the command arguments
    const MESPObject &keyObj = obj.arrayValue[1];
    const MESPObject &nameObject1 = obj.arrayValue[2];
    const MESPObject &nameObject2 = obj.arrayValue[3];

    // Verify that all extracted objects are of type BulkString
    if (keyObj.type == MESPType::BulkString && nameObject1.type == MESPType::BulkString && nameObject2.type == MESPType::BulkString)
    {
        std::string key = keyObj.stringValue;
        std::string location1 = nameObject1.stringValue;
        std::string location2 = nameObject2.stringValue;

        // Initialize GeoPoint objects for the two locations
        GeoPoint geoPoint1("", 0.0f, 0.0f, 0.0f); // Placeholder GeoPoint
        GeoPoint geoPoint2("", 0.0f, 0.0f, 0.0f); // Placeholder GeoPoint

        // Retrieve the geographic data from the cache
        bool found1 = geo_cache_->GetGeoPoint(key, location1, geoPoint1);
        bool found2 = geo_cache_->GetGeoPoint(key, location2, geoPoint2);

        // Check if both locations were found in the cache
        if (!found1 || !found2)
        {
            std::string res = "NOT FOUND: Location not found in Cache";
            MESPObject resObj(MESPType::BulkString, res);
            response = CommandParser::serializeResponse(resObj);
            return;
        }

        // Calculate the distance between the two locations
        double dist = geo_cache_->GetGeoDistance(geoPoint1, geoPoint2);

        // Create a response object with the calculated distance
        MESPObject distObj(MESPType::Float, static_cast<float>(dist));
        response = CommandParser::serializeResponse(distObj);
    }
    else
    {
        // Handle invalid command format if any object is not of type BulkString
        HandleInvalidCommandFormat(response);
    }
}