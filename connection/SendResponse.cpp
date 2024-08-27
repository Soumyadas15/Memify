#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

#include "ConnectionHandler.h"
#include "MessageProcessor.h"
#include "HMACUtil.h"
#include "LoggerManager.h"
#include "FileLogger.h"

/**
 * @brief Sends a response to the client over the socket.
 *
 * This method sends the response in a format that includes the length of the response followed by the actual response data. The length is sent first as a 4-byte integer in network byte order.
 *
 * @param response The response message to be sent to the client.
 */
void ConnectionHandler::SendResponse(const std::string &response)
{
    // Convert the length of the response to network byte order for transmission.
    uint32_t response_length = htonl(static_cast<uint32_t>(response.size()));
    // Send the length of the response.
    send(client_fd_, &response_length, sizeof(response_length), 0);
    // Send the actual response data.
    send(client_fd_, response.c_str(), response.size(), 0);
}