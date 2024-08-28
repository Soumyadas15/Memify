#include "Server.h"
#include <memory>

#include "Cache.h"
#include "GeoCache.h"
#include "TimeSeriesCache.h"

/**
 * @brief The entry point of the application.
 *
 * This main function initializes the necessary components for running the server.
 * It sets up a shared cache, creates a server instance, and starts the server
 * to listen for incoming client connections.
 *
 * @return int Returns 0 on successful execution, or a non-zero value in case of an error.
 */
int main()
{
    // Initialize a shared pointer to the Cache object.
    // This cache will be shared across multiple client connections to store and retrieve data efficiently.
    auto cache = std::make_shared<Cache>();
    auto geo_cache = std::make_shared<GeoCache>();
    auto time_series_cache = std::make_shared<TimeSeriesCache>();

    // Create a Server object, passing the shared cache and specifying the port number.
    // The server is set to listen on port 8080 by default.
    Server server(cache, geo_cache, time_series_cache, 8080);

    // Start the server to begin listening for incoming connections.
    // This method will block the main thread as it runs the server loop to handle clients.
    server.Start();

    // Note: In a more robust application, signal handling could be added here to
    // gracefully shut down the server on receiving termination signals (e.g., SIGINT).

    return 0;
}