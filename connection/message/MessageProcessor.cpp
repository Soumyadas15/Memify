#include "MessageProcessor.h"
#include <iostream>

#include <ICache.h>
#include <IGeoCache.h>
#include <ITimeSeriesCache.h>

#include "GeoPoint.h"
#include "CommandParser.h"

/**
 * @brief Constructs a MessageProcessor object with a given cache.
 *
 * The constructor initializes the MessageProcessor with a shared pointer to an ICache object.
 * This allows the MessageProcessor to interact with the cache for storing, retrieving, and deleting data.
 *
 * @param cache A shared pointer to an ICache object used for caching data.
 */
MessageProcessor::MessageProcessor(
    std::shared_ptr<ICache> cache,
    std::shared_ptr<IGeoCache> geo_cache,
    std::shared_ptr<ITimeSeriesCache> time_series_cache
) : cache_(std::move(cache)),
    geo_cache_(std::move(geo_cache)),
    time_series_cache_(std::move(time_series_cache)) {}

/**
 * @brief Handles an incoming message and generates an appropriate response.
 *
 * This method extracts a command from the input message and processes it accordingly.
 * It uses a string stream to parse the message and delegates command handling to the `HandleCommand` method.
 *
 * @param message The input message to be processed, expected to contain a command and arguments.
 * @param response The output response generated after processing the message.
 */
void MessageProcessor::HandleMessage(const std::string &message, std::string &response)
{
    try
    {
        std::string input = message; // Create a copy of the input message
        CommandParser parser;
        MESPObject MESPObject = parser.parse(input); // Parse the message

        
        // Handle the parsed RESP object
        HandleCommand(MESPObject, response);
    }
    catch (const std::exception &e)
    {
        // Handle parsing or processing errors
        response = "ERROR: " + std::string(e.what());
    }
}











/**
 * @brief Handles a parsed MESP command object and generates an appropriate response.
 *
 * This method processes a parsed MESP object based on its type and content. It identifies the command and
 * delegates to specific handlers based on the command type. If the command or format is invalid, it generates
 * an error response. It supports basic commands like "SET" and "GET" and checks for the validity of command format.
 *
 * @param obj A constant reference to the parsed MESPObject representing the command to be handled.
 * @param response A reference to the output string that will be populated with the response message.
 *
 * The method performs the following:
 * - **SimpleString Handling**: If the object is a SimpleString, it treats it as a "PING" command and invokes `HandlePing`.
 * - **Array Handling**: If the object is an Array, it checks if it is non-empty and processes the command based on the first element:
 *   - **Command Validation**: Checks if the first element of the array is a BulkString representing the command (e.g., "SET", "GET").
 *   - **Command Execution**:
 *     - **"SET" Command**: Delegates to `HandleSet` for handling the "SET" command.
 *     - **"GET" Command**: Delegates to `HandleGet` for handling the "GET" command.
 *     - **Invalid Commands**: Calls `HandleInvalidCommand` for unknown commands or invalid formats.
 * - **Error Handling**: If the object type is not recognized or the format is invalid, it calls `HandleInvalidRespType` to generate an error response.
 */
void MessageProcessor::HandleCommand(const MESPObject &obj, std::string &response)
{
    // Handle SimpleString type
    if (obj.type == MESPType::SimpleString)
    {
        HandlePing(response);
    }
    // Handle Array type
    else if (obj.type == MESPType::Array)
    {
        // Check if the array is empty
        if (obj.arrayValue.empty())
        {
            HandleInvalidCommand(response);
            return;
        }

        // Process the first element of the array as the command
        const MESPObject &commandObj = obj.arrayValue[0];

        // Ensure the first element is a BulkString
        if (commandObj.type != MESPType::BulkString)
        {
            HandleInvalidCommandFormat(response);
            return;
        }

        // Get the command string and delegate to appropriate handler
        std::string command = commandObj.stringValue;
        if (command == "SET")
        {
            HandleSet(obj, response);
        }
        else if (command == "GET")
        {
            HandleGet(obj, response);
        }
        else if (command == "DELETE")
        {
            HandleDelete(obj, response);
        }
        else if (command == "GEOSET")
        {
            HandleGeoSet(obj, response);
        }
        else if (command == "GEOGET")
        {
            HandleGeoGet(obj, response);
        }
        else if (command == "GEODISTANCE")
        {
            HandleGeoDistance(obj, response);
        }
        else
        {
            HandleInvalidCommand(response);
        }
    }
    else
    {
        HandleInvalidRespType(response);
    }
}