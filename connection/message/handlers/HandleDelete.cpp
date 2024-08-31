#include "MessageProcessor.h"
#include <iostream>


void MessageProcessor::HandleDelete(const MESPObject &obj, std::string &response)
{
    // Check if the command contains exactly two elements: the command itself and the key
    if (obj.arrayValue.size() != 2)
    {
        HandleInvalidCommandFormat(response);
        return;
    }

    // Extract the key from the command arguments
    const MESPObject &keyObj = obj.arrayValue[1];

    // Check if the key is of type BulkString
    if (keyObj.type == MESPType::BulkString)
    {
        std::string key = keyObj.stringValue;
        std::string value;

        // Retrieve the value associated with the key from the cache
        if (cache_->Get(key, value))
        {
            cache_->Delete(key);
            MESPObject resObj(MESPType::BulkString, "SUCCESS");
            std::string serializedResponse = CommandParser::serializeResponse(resObj);
            response = serializedResponse;
            return;
        }
        else
        {
            // Create a MESPObject with "NOT FOUND" and serialize it
            MESPObject resObj(MESPType::BulkString, "NOT FOUND");
            std::string serializedResponse = CommandParser::serializeResponse(resObj);
            response = serializedResponse;
            return;
        }
    }
    else
    {
        // Handle invalid command format if the key is not of type BulkString
        HandleInvalidCommandFormat(response);
        return;
    }
}