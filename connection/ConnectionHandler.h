#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <memory>
#include <string>
#include <vector>
#include <sstream>

#include "FileLogger.h"
#include "ICache.h"
#include "IGeoCache.h"
#include "ITimeSeriesCache.h"

/**
 * @class ConnectionHandler
 * @brief Manages the communication between a single client and the server.
 *
 * The `ConnectionHandler` class is designed to handle communication with a client connected to the server. It manages receiving and processing client messages, verifying their signatures, and sending responses back to the client. It also provides logging functionality to track connection details and operations.
 */
class ConnectionHandler
{
public:
    
    ConnectionHandler(
            std::shared_ptr<ICache> cache, 
            std::shared_ptr<IGeoCache> geo_cache, 
            std::shared_ptr<ITimeSeriesCache> time_series_cache, 
            int client_fd, 
            const std::string &secret_key
    );

    /**
     * @brief Handles the communication with the connected client.
     *
     * This method continuously reads data from the client socket, assembles complete messages, verifies their signatures, processes the messages, and sends responses. It handles different scenarios including data reception failures and client disconnections.
     */
    void HandleClient();

private:
    std::shared_ptr<ICache> cache_;           ///< A shared pointer to an `ICache` instance used for caching data.
    std::shared_ptr<IGeoCache> geo_cache_;           ///< A shared pointer to an `ICache` instance used for caching data.
    std::shared_ptr<ITimeSeriesCache> time_series_cache_; ///< A shared pointer to an `ICache` instance used for caching data.
    int client_fd_;                           ///< The file descriptor for the client connection.
    std::string secret_key_;                  ///< The secret key used for verifying message signatures.
    std::shared_ptr<FileLogger> file_logger_; ///< A shared pointer to a `FileLogger` instance for logging connection activities.

    /**
     * @brief Verifies the signature of the given payload using HMAC.
     *
     * This method checks whether the provided signature matches the HMAC of the payload data using the stored secret key. It ensures that the payload has not been tampered with during transmission.
     *
     * @param data The payload data whose signature needs to be verified.
     * @param signature The signature to verify against the payload data.
     * @return True if the signature matches the HMAC of the payload; otherwise, false.
     */
    bool VerifySignature(const std::string &data, const std::string &signature);

    /**
     * @brief Processes the incoming message and generates a response.
     *
     * This method handles the business logic of processing the message. It interacts with the cache and prepares the appropriate response based on the message content.
     *
     * @param message The message payload that needs to be processed.
     * @param response A reference to a string where the processed response will be stored.
     */
    void ProcessMessage(const std::string &message, std::string &response);

    /**
     * @brief Sends a response to the client over the socket.
     *
     * This method sends the response to the client in a format that includes the length of the response followed by the actual response data. The length is sent first as a 4-byte integer in network byte order.
     *
     * @param response The response message to be sent to the client.
     */
    void SendResponse(const std::string &response);
};

#endif // CONNECTIONHANDLER_H