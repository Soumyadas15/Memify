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
 * @brief Handles the communication with the connected client.
 *
 * This method is responsible for continuously reading data from the client socket, assembling complete messages from chunks of data, verifying their signatures, processing the messages, and sending responses. It handles different scenarios including data reception failures and client disconnections.
 *
 * The communication loop performs the following steps:
 * 1. Receives data chunks from the client.
 * 2. Accumulates the received data in a buffer.
 * 3. Extracts message length and processes messages once fully received.
 * 4. Verifies message signatures and processes valid messages.
 * 5. Sends appropriate responses to the client.
 * 6. Logs errors and disconnections.
 */
void ConnectionHandler::HandleClient()
{
    LoggerManager::getInstance().info("Client connected");

    std::vector<char> buffer;     ///< Buffer to accumulate incoming data from the client.
    uint32_t expected_length = 0; ///< Length of the next expected message (in bytes).

    while (true)
    {
        char chunk[1024]; ///< Temporary buffer to receive data from the client.
        ssize_t bytes_received = recv(client_fd_, chunk, sizeof(chunk), 0);

        // Handle errors or client disconnections.
        if (bytes_received <= 0)
        {
            if (bytes_received < 0)
            {
                // Log an error if the data reception failed.
                std::cout << "Recv failed" << std::endl;
                file_logger_->error("Recv failed: " + std::string(strerror(errno)));
            }
            else
            {
                // Log that the client has disconnected normally.
                std::cout << "Client disconnected" << std::endl;
                file_logger_->info("Client disconnected");
            }
            break; // Exit the loop when there is an error or the client disconnects.
        }

        // Append the received chunk of data to the buffer.
        buffer.insert(buffer.end(), chunk, chunk + bytes_received);

        while (!buffer.empty())
        {
            // If the expected length of the message is not known, extract it from the buffer.
            if (expected_length == 0)
            {
                if (buffer.size() < 4)
                    break; // Wait for more data if the buffer doesn't have enough bytes to extract the length prefix.

                // Extract the message length prefix (4 bytes) and update the expected length of the message.
                expected_length = ntohl(*reinterpret_cast<uint32_t *>(buffer.data()));
                buffer.erase(buffer.begin(), buffer.begin() + 4);
            }

            // If the buffer does not have enough data to complete the message, wait for more data.
            if (buffer.size() < expected_length)
                break;

            // Extract the complete message from the buffer.
            std::string message(buffer.begin(), buffer.begin() + expected_length);
            buffer.erase(buffer.begin(), buffer.begin() + expected_length);
            expected_length = 0; // Reset the expected length for the next message.

            // Find the delimiter that separates the signature from the payload.
            size_t delimiter_pos = message.find('\n');
            if (delimiter_pos == std::string::npos)
            {
                // Log an error if the message format is invalid (missing delimiter).
                file_logger_->error("Invalid message format: " + std::string(strerror(errno)));
                SendResponse("Invalid message format");
                continue;
            }

            // Extract the signature and payload from the message.
            std::string signature = message.substr(0, delimiter_pos);
            std::string payload = message.substr(delimiter_pos + 1);

            // Verify the signature of the payload to ensure its integrity.
            if (VerifySignature(payload, signature))
            {
                // Process the valid message and prepare a response.
                std::string response;
                ProcessMessage(payload, response);
                // Send the response back to the client.
                SendResponse(response);
            }
            else
            {
                // Log an error if signature verification fails and send an error response.
                file_logger_->error("Signature Verification Failure: " + std::string(strerror(errno)));
                SendResponse("Signature Verification Failure");
            }
        }
    }

    // Close the client connection once the communication loop ends.
    close(client_fd_);
}