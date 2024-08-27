#ifndef SERVER_H
#define SERVER_H

#include "ICache.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <atomic>

/**
 * @class Server
 * @brief A class representing a server that handles client requests.
 *
 * The `Server` class is responsible for managing client connections, handling requests,
 * and interfacing with a cache system. It runs on a specified port and can be started or stopped
 * programmatically. The server is designed to be thread-safe and uses atomic operations to manage
 * its running state.
 */
class Server
{
public:
    /**
     * @brief Constructs a Server object with a given cache and port.
     *
     * @param cache A shared pointer to an `ICache` object, providing the caching mechanism.
     * @param port The port number on which the server will listen for incoming connections.
     */
    Server(std::shared_ptr<ICache> cache, uint16_t port);

    /**
     * @brief Starts the server, allowing it to accept and handle client connections.
     *
     * This method initializes the server socket, binds it to the specified port,
     * and begins accepting connections in a loop until the server is stopped.
     */
    void Start();

    /**
     * @brief Stops the server, terminating the acceptance of new client connections.
     *
     * This method sets the running state to false, allowing the server to gracefully
     * shutdown. Any ongoing client connections will be handled before the server stops completely.
     */
    void Stop();

private:
    std::shared_ptr<ICache> cache_; ///< Shared pointer to a cache object for storing and retrieving data.
    uint16_t port_;                 ///< The port number on which the server listens for connections.
    std::string secret_key_;        ///< A secret key used for security purposes, such as HMAC validation.
    std::atomic<bool> running_;     ///< Atomic boolean flag to indicate the running state of the server.

    /**
     * @brief Handles communication with a connected client.
     *
     * This method is called for each client connection and is responsible for reading the client's
     * request, processing it, and sending back a response. The handling of each client is done in a
     * separate thread to allow for concurrent client processing.
     *
     * @param client_fd The file descriptor for the client's socket connection.
     */
    void HandleClient(int client_fd);
};

#endif // SERVER_H