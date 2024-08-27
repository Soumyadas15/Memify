#include "ResponseSender.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * @brief Sends a response message to a client identified by the file descriptor.
 *
 * This method takes a client's file descriptor and a response string, then sends the response
 * in two parts:
 * 1. The length of the response message, sent as a 32-bit integer in network byte order.
 * 2. The actual response message.
 *
 * This ensures that the receiving end knows how many bytes to expect for the message,
 * which helps in correctly reconstructing the message from the byte stream.
 *
 * @param client_fd An integer representing the client's file descriptor, used to identify the connection.
 * @param response A string containing the response message to be sent to the client.
 */
void ResponseSender::Send(int client_fd, const std::string &response)
{
    // Convert the response length to network byte order (big-endian) for transmission
    uint32_t response_length = htonl(static_cast<uint32_t>(response.size()));

    // Send the length of the response message first
    send(client_fd, &response_length, sizeof(response_length), 0);

    // Send the actual response message
    send(client_fd, response.c_str(), response.size(), 0);
}