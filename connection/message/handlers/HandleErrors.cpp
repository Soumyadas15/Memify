#include "MessageProcessor.h"
#include <iostream>

/**
 * @brief Handles an invalid command by setting the response to "INVALID COMMAND".
 *
 * This method is used to indicate that the command received is not recognized or supported by the server.
 *
 * @param response A reference to the output string that will be populated with the response message.
 *
 * The method performs the following:
 * - Sets the `response` string to "INVALID COMMAND".
 */
void MessageProcessor::HandleInvalidCommand(std::string &response)
{
    std::string res = "INVALID COMMAND: Invalid command";

    MESPObject resObj(MESPType::BulkString, res);
    std::string serializedResponse = CommandParser::serializeResponse(resObj);
    response = serializedResponse;
}

/**
 * @brief Handles an invalid command format by setting the response to "INVALID COMMAND FORMAT".
 *
 * This method is used to indicate that the format of the command received is incorrect or does not adhere to the expected structure.
 *
 * @param response A reference to the output string that will be populated with the response message.
 *
 * The method performs the following:
 * - Sets the `response` string to "INVALID COMMAND FORMAT".
 */
void MessageProcessor::HandleInvalidCommandFormat(std::string &response)
{

    std::string res = "INVALID COMMAND: Invalid command format";

    MESPObject resObj(MESPType::BulkString, res);
    std::string serializedResponse = CommandParser::serializeResponse(resObj);
    response = serializedResponse;
}

/**
 * @brief Handles an invalid RESP type by setting the response to "INVALID RESP TYPE".
 *
 * This method is used to indicate that the RESP type received is not recognized or does not match the expected types.
 *
 * @param response A reference to the output string that will be populated with the response message.
 *
 * The method performs the following:
 * - Sets the `response` string to "INVALID RESP TYPE".
 */
void MessageProcessor::HandleInvalidRespType(std::string &response)
{
    std::string res = "INVALID COMMAND: Invalid MESP type";

    MESPObject resObj(MESPType::BulkString, res);
    std::string serializedResponse = CommandParser::serializeResponse(resObj);
    response = serializedResponse;
}