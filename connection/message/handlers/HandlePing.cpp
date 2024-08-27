#include "MessageProcessor.h"
#include <iostream>

/**
 * @brief Handles the "PING" command by generating a "PONG" response.
 *
 * This method processes the "PING" command by setting the response string to "PONG".
 * It is used to verify that the server is operational and responsive.
 *
 * @param response A reference to the output string that will be populated with the response message.
 *
 * The method performs the following:
 * - Sets the response string to "PONG".
 */
void MessageProcessor::HandlePing(std::string &response)
{
    response = "PONG";
}