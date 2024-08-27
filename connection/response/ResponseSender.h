#ifndef RESPONSESENDER_H
#define RESPONSESENDER_H

#include <string>

/**
 * @class ResponseSender
 * @brief A utility class responsible for sending responses to connected client.
 *
 * The ResponseSender class provides a static method to send a response message to a client using a file descriptor.
 * This is commonly used in network programming where each client connection is represented by a file descriptor.
 */
class ResponseSender
{
public:
    /**
     * @brief Sends a response message to a client identified by the file descriptor.
     *
     * This static method takes a client file descriptor and a response string, then sends the response
     * to the client over the network connection associated with the file descriptor. It handles the
     * transmission of the response message, ensuring that the entire message is sent.
     *
     * @param client_fd An integer representing the client's file descriptor, used to identify the connection.
     * @param response A string containing the response message to be sent to the client.
     */
    static void Send(int client_fd, const std::string &response);
};

#endif // RESPONSESENDER_H