#include "MessageProcessor.h"
#include <iostream>
#include <stdexcept>

/**
 * @brief Handles the geo set command by updating or adding geographic information for a specific location.
 *
 * This function processes a command to set geographic details (name, latitude, longitude, and elevation)
 * for a specific location. It validates the command format, updates the geo cache with the new data,
 * and formats the response accordingly.
 *
 * @param obj A constant reference to an `MESPObject` that represents the command object.
 *            This object contains an array of command arguments.
 * @param response A reference to a `std::string` where the response will be stored and returned.
 *
 * The expected command format is:
 *  - Command (e.g., "GEOSET")
 *  - Key (BulkString)
 *  - Value (BulkString)
 *  - Latitude (Float)
 *  - Longitude (Float)
 *  - Elevation (Float, optional)
 *
 * If the command format is invalid or if the geographic point format is incorrect, an error message is returned.
 * If the command is valid and the data is successfully updated in the cache, the updated details are returned.
 */
void MessageProcessor::HandleGeoSet(const MESPObject &obj, std::string &response)
{
    // Validate that the command has at least five elements (GEOSET, key, value, latitude, longitude)
    if (obj.arrayValue.size() < 5 || obj.arrayValue.size() > 6)
    {
        HandleInvalidCommandFormat(response);
        return;
    }

    // Extract key, value, latitude, longitude, and optionally elevation from the command
    const MESPObject &keyObj = obj.arrayValue[1];
    const MESPObject &valueObj = obj.arrayValue[2];
    const MESPObject &latitudeObj = obj.arrayValue[3];
    const MESPObject &longitudeObj = obj.arrayValue[4];
    const MESPObject &elevationObj = (obj.arrayValue.size() == 6) ? obj.arrayValue[5] : MESPObject();

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
            double elevation = (elevationObj.type == MESPType::Float) ? elevationObj.floatValue : 0.0;

            // Create GeoPoint object
            GeoPoint geoPoint(value, latitude, longitude, elevation);

            // Update the geo cache with the GeoPoint
            geo_cache_->SetGeoPoint(key, geoPoint);

            // Create response objects for the updated geographic details
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
            std::string res = "INVALID COMMAND: Invalid geopoint format";
            MESPObject resObj(MESPType::BulkString, res);
            response = CommandParser::serializeResponse(resObj);
            return;
        }
    }
    else
    {
        // Handle invalid command format if key or value is not of type BulkString
        HandleInvalidCommandFormat(response);
    }
}