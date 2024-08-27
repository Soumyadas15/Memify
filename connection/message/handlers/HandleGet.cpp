#include "MessageProcessor.h"
#include <iostream>

/**
 * @brief Handles the "GET" command by retrieving the value associated with the specified key from the cache.
 *
 * This method processes the "GET" command by checking if the correct number of arguments are provided and if the key is in the expected format.
 * It then retrieves the value for the given key from the cache and sets the response string accordingly.
 * If the key is found in the cache, the corresponding value is set in the response. If the key is not found, "NOT FOUND" is returned.
 *
 * @param obj The MESPObject representing the parsed command and its arguments.
 * @param response A reference to the output string that will be populated with the response message.
 *
 * The method performs the following:
 * - Validates the number of arguments in the command.
 * - Checks if the key is of type `BulkString`.
 * - Retrieves the value for the key from the cache.
 * - Sets the `response` to the retrieved value or "NOT FOUND" if the key is not present.
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
            // Set the response to the retrieved value
            response = value;
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