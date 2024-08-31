#include "MessageProcessor.h"
#include <iostream>

/**
 * @brief Handles the "PING" command by generating a "PONG" response.
 *
 * This method processes the "PING" command and sets the response string to "PONG".
 * The "PING" command is typically used to verify that the server is operational and responsive.
 *
 * @param response A reference to a `std::string` where the "PONG" response message will be stored and returned.
 *
 * The method performs the following steps:
 * - Creates a `MESPObject` with type `BulkString` and value "PONG".
 * - Serializes this `MESPObject` into a string format using `CommandParser::serializeResponse`.
 * - Assigns the serialized string to the `response` parameter.
 *
 * Example:
 * If the server receives a "PING" command, the response will be "PONG".
 */
void MessageProcessor::HandlePing(std::string &response)
{
    // Create a MESPObject with type BulkString and value "PONG"
    MESPObject resObj(MESPType::BulkString, "PONG");

    // Serialize the MESPObject into a string format
    std::string serializedResponse = CommandParser::serializeResponse(resObj);

    // Set the response parameter to the serialized string
    response = serializedResponse;
    return;
}