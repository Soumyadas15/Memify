#include "MessageProcessor.h"
#include "CommandParser.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

/**
 * @brief Handles the geo get command by retrieving geographic information for a specific location.
 *
 * This function processes a command to retrieve geographic details (name, latitude, longitude, and elevation)
 * for a specific location. It checks the format of the command, retrieves the geographic data from the cache,
 * and formats the response accordingly.
 *
 * @param obj A constant reference to an `MESPObject` that represents the command object.
 *            This object contains an array of command arguments.
 * @param response A reference to a `std::string` where the response will be stored and returned.
 *
 * The expected command format is:
 *  - Command (e.g., "GEOGGET")
 *  - Key (BulkString)
 *  - Location Name (BulkString)
 *
 * If the command format is invalid or the location is not found in the cache, an error message is returned.
 * If the command is valid and the location is found, the geographic details are returned.
 */
void MessageProcessor::HandleGeoGet(const MESPObject &obj, std::string &response)
{
    // Check if the command contains exactly three elements
    if (obj.arrayValue.size() != 3)
    {
        HandleInvalidCommandFormat(response);
        return;
    }

    // Extract the key and location name from the command arguments
    const MESPObject &keyObj = obj.arrayValue[1];
    const MESPObject &nameObj = obj.arrayValue[2];

    // Check if both extracted objects are of type BulkString
    if (keyObj.type == MESPType::BulkString && nameObj.type == MESPType::BulkString)
    {
        std::string key = keyObj.stringValue;
        std::string nameKey = nameObj.stringValue;
        std::string name;
        float latitude;
        float longitude;
        float elevation;
        GeoPoint geoPoint(name, latitude, longitude, elevation);

        // Retrieve the geographic data associated with the key and location name from the cache
        if (geo_cache_->GetGeoPoint(key, nameKey, geoPoint))
        {
            // Create response objects for the retrieved geographic details
            MESPObject nameObj(MESPType::BulkString, geoPoint.name);
            MESPObject latObj(MESPType::Float, geoPoint.latitude);
            MESPObject lonObj(MESPType::Float, geoPoint.longitude);
            MESPObject elevObj(MESPType::Float, geoPoint.elevation);

            // Combine all response objects into a response array
            std::vector<MESPObject> responseArray = {nameObj, latObj, lonObj, elevObj};
            MESPObject responseObj(MESPType::Array, responseArray);

            // Serialize the response object to a string
            std::string serializedResponse = CommandParser::serializeResponse(responseObj);

            response = serializedResponse;
            return;
        }
        else
        {
            // Handle the case where the location was not found in the cache
            MESPObject notFoundResponse(MESPType::SimpleString, "NOT FOUND: Location not found in cache");
            std::string serializedNotFound = CommandParser::serializeResponse(notFoundResponse);
            response = serializedNotFound;
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