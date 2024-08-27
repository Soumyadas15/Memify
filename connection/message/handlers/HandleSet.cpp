#include "MessageProcessor.h"
#include <iostream>

/**
 * @brief Handles the "SET" command and updates the cache with the provided key-value pair and optional duration.
 *
 * This method processes the "SET" command by extracting the key, value, and optional duration from the provided MESP object.
 * It updates the cache with the specified key-value pair and sets an expiration duration if provided. If the format is
 * incorrect or the required elements are missing, it generates an appropriate error response.
 *
 * @param obj A constant reference to the MESPObject representing the "SET" command and its arguments.
 * @param response A reference to the output string that will be populated with the response message.
 *
 * The method performs the following:
 * - **Command Validation**: Ensures that the array representing the command has at least three elements (command, key, and value).
 * - **Argument Extraction**:
 *   - Extracts the key and value from the array if they are of type `BulkString`.
 *   - Checks if the array includes an optional duration element.
 * - **Duration Handling**:
 *   - If a duration is provided, verifies that it is of type `Integer` and converts it to `std::chrono::seconds`.
 *   - If the duration format is invalid, generates an error response.
 * - **Cache Update**: Uses the `cache_` object to store the key-value pair with the specified duration.
 * - **Response Generation**:
 *   - Sets the response to "OK" if the operation is successful.
 *   - Calls `HandleInvalidCommandFormat` for incorrect formats or missing elements.
 */
void MessageProcessor::HandleSet(const MESPObject &obj, std::string &response)
{
    // Validate that the command has at least 3 elements (command, key, value)
    if (obj.arrayValue.size() < 3)
    {
        HandleInvalidCommandFormat(response);
        return;
    }

    // Extract key and value from the command
    const MESPObject &keyObj = obj.arrayValue[1];
    const MESPObject &valueObj = obj.arrayValue[2];

    // Check if key and value are of type BulkString
    if (keyObj.type == MESPType::BulkString && valueObj.type == MESPType::BulkString)
    {
        std::string key = keyObj.stringValue;
        std::string value = valueObj.stringValue;

        std::chrono::seconds duration = std::chrono::seconds(0);

        // Check if an optional duration is provided
        if (obj.arrayValue.size() == 4)
        {
            const MESPObject &durationObj = obj.arrayValue[3];
            if (durationObj.type == MESPType::Integer)
            {
                duration = std::chrono::seconds(durationObj.intValue);
            }
            else
            {
                response = "INVALID DURATION FORMAT";
                return;
            }
        }
        // Update the cache with the key, value, and duration
        cache_->Set(key, value, duration);
        response = "OK";
    }
    else
    {
        HandleInvalidCommandFormat(response);
    }
}