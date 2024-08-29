#include "MessageProcessor.h"
#include <iostream>
#include <stdexcept>

void MessageProcessor::HandleGeoDistance(const MESPObject &obj, std::string &response)
{
    // Check if the command contains exactly two elements: the command itself and the key
    if (obj.arrayValue.size() < 4 || obj.arrayValue.size() > 5)
    {
        HandleInvalidCommandFormat(response);
        return;
    }

    // Extract the key from the command arguments
    const MESPObject &keyObj = obj.arrayValue[1];
    const MESPObject &nameObject1 = obj.arrayValue[2];
    const MESPObject &nameObject2 = obj.arrayValue[3];

    // Check if the key is of type BulkString
    if (keyObj.type == MESPType::BulkString && nameObject1.type == MESPType::BulkString && nameObject2.type == MESPType::BulkString)
    {
        std::string key = keyObj.stringValue;
        std::string location1 = nameObject1.stringValue;
        std::string location2 = nameObject2.stringValue;


        std::string name;
        float latitude1, latitude2;
        float longitude1, longitude2;
        float elevation1, elevation2;

        GeoPoint geoPoint1(name, latitude1, longitude1, elevation1);
        GeoPoint geoPoint2(name, latitude2, longitude2, elevation2);

        // Retrieve the value associated with the key from the gep cache
        auto x = geo_cache_->GetGeoPoint(key, location1, geoPoint1);
        auto y = geo_cache_->GetGeoPoint(key, location2, geoPoint2);

        if(!x || !y){
            std::cout << "Not found" << std::endl;
            response = "Error: Location not found in Cache";
            return;
        }
        else if (x && y)
        {
            double dist = geo_cache_->GetGeoDistance(geoPoint1, geoPoint2);
            response = std::to_string(dist);
        }
        else
        {
            response = "NOT FOUND";
        }
    }
    else
    {
        // Handle invalid command format if the key is not of type BulkString
        HandleInvalidCommandFormat(response);
    }
}