#include "MessageProcessor.h"
#include <iostream>
#include <stdexcept>

/**
 * @brief Handles the geo distance command by calculating the distance between two locations.
 *
 * This function processes a command to calculate the geographic distance between two locations.
 * It validates the command format, retrieves geographic data from a cache, calculates the distance,
 * and formats the response accordingly.
 *
 * @param obj A constant reference to an `MESPObject` that represents the command object.
 *            This object contains an array of command arguments.
 * @param response A reference to a `std::string` where the response will be stored and returned.
 *
 * The expected command format is:
 *  - Command (e.g., "GEODISTANCE")
 *  - Key (BulkString)
 *  - Location 1 (BulkString)
 *  - Location 2 (BulkString)
 *
 * If the command format is invalid or locations are not found in the cache, an error message is returned.
 * If the command is valid and locations are found, the distance between the locations is calculated and returned.
 */
void MessageProcessor::HandleGeoDistance(const MESPObject &obj, std::string &response)
{
    // Check if the command contains exactly four or five elements
    if (obj.arrayValue.size() < 4 || obj.arrayValue.size() > 5)
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
        std::string name;
        float latitude1, latitude2;
        float longitude1, longitude2;
        float elevation1, elevation2;

        GeoPoint geoPoint1(name, latitude1, longitude1, elevation1);
        GeoPoint geoPoint2(name, latitude2, longitude2, elevation2);

        // Retrieve the geographic data from the cache
        auto x = geo_cache_->GetGeoPoint(key, location1, geoPoint1);
        auto y = geo_cache_->GetGeoPoint(key, location2, geoPoint2);

        // Check if both locations were found in the cache
        if (!x || !y)
        {
            std::string res = "NOT FOUND: Location not found in Cache";
            MESPObject resObj(MESPType::BulkString, res);
            response = CommandParser::serializeResponse(resObj);
            return;
        }
        else if (x && y)
        {
            // Calculate the distance between the two locations
            double dist = geo_cache_->GetGeoDistance(geoPoint1, geoPoint2);

            // Create a response object with the calculated distance
            MESPObject distObj(MESPType::Float, static_cast<float>(dist));
            response = CommandParser::serializeResponse(distObj);
            return;
        }
    }
    else
    {
        // Handle invalid command format if any object is not of type BulkString
        HandleInvalidCommandFormat(response);
        return;
    }
}