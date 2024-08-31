#include "MessageProcessor.h"
#include <iostream>

/**
 * @brief Handles the "SET" command and updates the cache with the provided key-value pair and optional duration.
 *
 * This method processes the "SET" command by extracting the key, value, and an optional expiration duration from the provided MESP object.
 * It updates the cache with the specified key-value pair and sets an expiration duration if provided. If the format is incorrect or
 * the required elements are missing, it generates an appropriate error response.
 *
 * @param obj A constant reference to the `MESPObject` representing the "SET" command and its arguments.
 * @param response A reference to the output string that will be populated with the response message.
 *
 * The method performs the following steps:
 * - **Command Validation**: Ensures that the `arrayValue` in `obj` has at least three elements (the command itself, the key, and the value).
 * - **Argument Extraction**:
 *   - Extracts the `key` and `value` from the `arrayValue` if they are of type `BulkString`.
 *   - Checks if the `arrayValue` includes an optional duration element.
 * - **Duration Handling**:
 *   - If a duration is provided (i.e., if the `arrayValue` has four elements), it verifies that it is of type `Integer` and converts it to `std::chrono::seconds`.
 *   - If the duration format is invalid, sets the `response` to "INVALID DURATION FORMAT" and exits.
 * - **Cache Update**:
 *   - Uses the `cache_` object to store the `key`, `value`, and `duration` in the cache.
 * - **Response Generation**:
 *   - Creates `MESPObject` instances for the `key`, `value`, and `duration`.
 *   - Constructs a response array containing these objects.
 *   - Serializes the response array into a string using `CommandParser::serializeResponse`.
 *   - Sets the `response` parameter to the serialized string.
 * - **Error Handling**:
 *   - Calls `HandleInvalidCommandFormat` if the `key` or `value` is not of type `BulkString`, or if the number of elements is incorrect.
 *
 * Example:
 * - For a command with three elements: `"SET"`, `"mykey"`, and `"myvalue"`, the cache is updated with `"mykey"` and `"myvalue"`, and the response is "OK".
 * - For a command with four elements: `"SET"`, `"mykey"`, `"myvalue"`, and `60`, the cache is updated with `"mykey"`, `"myvalue"`, and an expiration duration of 60 seconds. The response includes these details.
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

        // Create MESPObjects for the key, value, and duration
        MESPObject keyMespObj(MESPType::BulkString, key);
        MESPObject valMespObj(MESPType::BulkString, value);
        MESPObject durationMespObj(MESPType::Integer, duration.count());

        // Construct a response array with the MESPObjects
        std::vector<MESPObject> responseArray = {keyMespObj, valMespObj, durationMespObj};
        MESPObject responseObj(MESPType::Array, responseArray);

        // Serialize the response array into a string
        std::string serializedResponse = CommandParser::serializeResponse(responseObj);

        // Set the response parameter to the serialized string
        response = serializedResponse;
        return;
    }
    else
    {
        // Handle invalid command format if the key or value is not of type BulkString
        HandleInvalidCommandFormat(response);
        return;
    }
}