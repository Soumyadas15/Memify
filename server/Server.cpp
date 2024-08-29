#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <thread>


#include "Server.h"
#include "ConnectionHandler.h"
#include "INIReader.h"


/**
 * @brief Constructs a Server object.
 *
 * Initializes the Server with a shared cache, port, and secret key. It reads configurations
 * from a 'config.ini' file if available. The default port and secret key can be overridden
 * by values specified in the 'config.ini'.
 *
 * @param cache Shared pointer to an ICache object for caching functionalities.
 * @param port Port number on which the server listens.
 */
Server::Server(std::shared_ptr<ICache> cache,
               std::shared_ptr<IGeoCache> geo_cache,
               std::shared_ptr<ITimeSeriesCache> time_series_cache,
               uint16_t port
)
    : cache_(std::move(cache)),
      geo_cache_(std::move(geo_cache)),
      time_series_cache_(std::move(time_series_cache)),
      port_(port),
      secret_key_("xyz"),
      running_(false)
{
    // Create an INIReader to read the configuration file.
    INIReader reader("../config.ini");

    // Check if the config file is loaded successfully.
    if (reader.ParseError() < 0)
    {
        std::cerr << "Can't load 'config.ini'" << std::endl;
        return;
    }

    // Override default port and secret_key with values from config.ini.
    port_ = reader.GetInteger("settings", "port", port_);
    secret_key_ = reader.Get("settings", "secret_key", secret_key_);
}

/**
 * @brief Starts the server to listen for incoming client connections.
 *
 * The server sets up a socket to listen on the specified port. It uses a loop to accept
 * incoming connections and spawns a new thread to handle each client using a ConnectionHandler.
 *
 * It supports the graceful shutdown of connections and handles errors during socket operations.
 */
void Server::Start()
{
    // Create a socket for the server.
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }

    // Set socket options to allow address reuse.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
        close(server_fd);
        return;
    }

    // Initialize server address structure.
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;                           // IPv4
    server_addr.sin_port = htons(static_cast<uint16_t>(port_)); // Port number
    server_addr.sin_addr.s_addr = INADDR_ANY;                   // Any incoming interface

    // Bind the socket to the specified port.
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        close(server_fd);
        return;
    }

    // Start listening for incoming connections.
    if (listen(server_fd, SOMAXCONN) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return;
    }

    std::cout << "Memify is listening on port " << port_ << std::endl;

    // Main loop to accept client connections.
    running_ = true;
    while (running_)
    {
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);

        // Accept an incoming client connection.
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0)
        {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "Client attempting to connect" << std::endl;

        if(AuthenticateClient(client_fd))
        {
            std::cout << "Client authenticated and connected" << std::endl;
            std::thread(&ConnectionHandler::HandleClient, ConnectionHandler(cache_, geo_cache_, time_series_cache_, client_fd, secret_key_)).detach();
        }
        else
        {
            std::cout << "Client authentication failed. Closing connection." << std::endl;
            close(client_fd);
        }
    }

    // Close the server socket once the server stops.
    close(server_fd);
}

/**
 * @brief Stops the server from accepting new connections.
 *
 * Sets the running state to false, which allows the main loop in the Start method
 * to terminate, leading to the shutdown of the server.
 */
void Server::Stop()
{
    running_ = false;
}