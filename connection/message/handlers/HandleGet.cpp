#include "MessageProcessor.h"
#include <iostream>

/**
 * @brief Handles the "GET" command by retrieving the value associated with the specified key from the cache.
 *
 * This method processes the "GET" command by verifying the command format and extracting the key from the command arguments.
 * It then retrieves the value associated with the key from the cache and sets the response string accordingly.
 * If the key is found in the cache, the corresponding value is set in the response. If the key is not found, "NOT FOUND" is returned.
 *
 * @param obj A constant reference to an `MESPObject` that represents the parsed command and its arguments.
 *            This object should contain the command and the key.
 * @param response A reference to a `std::string` where the response message will be stored and returned.
 *
 * The method performs the following steps:
 * - Checks if the command contains exactly two elements: the command itself and the key.
 * - Validates that the key is of type `BulkString`.
 * - Retrieves the value for the given key from the cache.
 * - Sets the `response` to the retrieved value if the key is present in the cache; otherwise, sets it to "NOT FOUND".
 *
 * Possible command formats:
 *  - Valid format: [GET, key] where `key` is a `BulkString`.
 *  - Invalid format: The command contains more or fewer than two elements, or the `key` is not a `BulkString`.
 */
void MessageProcessor::HandleGet(const MESPObject &obj, std::string &response)
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
            // Create a MESPObject with the retrieved value and serialize it
            MESPObject resObj(MESPType::BulkString, value);
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