#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <memory>
#include <string>
#include <sstream>
#include <optional>

#include <ICache.h>
#include <IGeoCache.h>
#include <ITimeSeriesCache.h>

#include "CommandParser.h"

/**
 * @class MessageProcessor
 * @brief Handles the processing of messages and execution of specific commands.
 *
 * The MessageProcessor class is responsible for interpreting incoming messages,
 * handling commands, and interacting with a cache to store or retrieve data.
 * It parses commands, executes appropriate actions, and generates responses based on the cache state.
 */
class MessageProcessor
{
public:
    /**
     * @brief Constructs a MessageProcessor object.
     *
     * Initializes the MessageProcessor with a shared pointer to an ICache object.
     * This allows the MessageProcessor to use the cache for storing, retrieving, and managing data.
     *
     * @param cache A shared pointer to an ICache object used for caching purposes.
     */
    explicit MessageProcessor(
        std::shared_ptr<ICache> cache,
        std::shared_ptr<IGeoCache> geo_cache,
        std::shared_ptr<ITimeSeriesCache> time_series_cache);

    /**
     * @brief Processes an incoming message and generates a response.
     *
     * Parses the input message to identify commands, processes the commands using the cache,
     * and constructs an appropriate response based on the command and cache operations.
     *
     * @param message The input message to be processed, which contains commands and arguments.
     * @param response The output string that will contain the response after processing the message.
     */
    void HandleMessage(const std::string &message, std::string &response);

private:
    std::shared_ptr<ICache> cache_; /**< Shared pointer to an ICache object for managing cached data. */
    std::shared_ptr<IGeoCache> geo_cache_; /**< Shared pointer to an IGeoCache object for managing cached data. */
    std::shared_ptr<ITimeSeriesCache> time_series_cache_; /**< Shared pointer to an IGeoCache object for managing cached data. */

    /**
     * @brief Processes the parsed RESP object and handles the specific command.
     *
     * Interprets the parsed RESP object, determines the type of command, and delegates
     * the command handling to the appropriate method based on the command type.
     *
     * @param obj The parsed RESP object representing the command and its arguments.
     * @param response The response string to be populated based on the command's execution.
     */
    void HandleCommand(const MESPObject &obj, std::string &response);






    /**
     * @brief Handles the "PING" command.
     *
     * Sets the response to "PONG" in response to a PING command.
     *
     * @param response The response string to be set to "PONG".
     */
    void HandlePing(std::string &response);








    /**
     * @brief Handles the "SET" command.
     *
     * Parses the arguments for the SET command and stores the key-value pair in the cache.
     * Optionally accepts a duration for expiration.
     *
     * @param obj The parsed RESP object containing the SET command and its arguments.
     * @param response The response string to be populated based on the execution of the SET command.
     */
    void HandleSet(const MESPObject &obj, std::string &response);

    /**
     * @brief Handles the "GET" command.
     *
     * Retrieves the value associated with the provided key from the cache.
     * Sets the response to the value if found, or "NOT FOUND" if the key does not exist.
     *
     * @param obj The parsed RESP object containing the GET command and its arguments.
     * @param response The response string to be set to the retrieved value or "NOT FOUND".
     */
    void HandleGet(const MESPObject &obj, std::string &response);

    /**
     * @brief Handles the "DELETE" command.
     *
     * Deletes the value associated with the provided key from the cache.
     * Sets the response to the "SUCCESS" if found, or "NOT FOUND" if the key does not exist.
     *
     * @param obj The parsed RESP object containing the DELETE command and its arguments.
     * @param response The response string to be set to "SUCCESS" or "NOT FOUND".
     */
    void HandleDelete(const MESPObject &obj, std::string &response);











    /**
     * @brief Handles the "GEOSET" command.
     *
     * Parses the arguments for the GEOSET command and stores the location key, name and its coordinates in the cache.
     * Optionally accepts a duration for expiration.
     *
     * @param obj The parsed MESP object containing the GEOSET command and its arguments.
     * @param response The response string to be populated based on the execution of the GEOSET command.
     */
    void HandleGeoSet(const MESPObject &obj, std::string &response);

    /**
     * @brief Handles the "GEODISTANCE" command.
     *
     * Parses the arguments for the GEODISTANCE command and stores the location key, name and its coordinates in the cache.
     * Optionally accepts a duration for expiration.
     *
     * @param obj The parsed MESP object containing the GEODISTANCE command and its arguments.
     * @param response The response string to be populated based on the execution of the GEODISTANCE command.
     */
    void HandleGeoDistance(const MESPObject &obj, std::string &response);

    /**
     * @brief Handles the "GEOGET" command.
     *
     * Parses the arguments for the GEOGET command and finds the name and cordinates of a place by given key.
     * Optionally accepts a duration for expiration.
     *
     * @param obj The parsed MESP object containing the GEOGET command and its arguments.
     * @param response The response string to be populated based on the execution of the GEOGET command.
     */
    void HandleGeoGet(const MESPObject &obj, std::string &response);














    /**
     * @brief Handles invalid commands.
     *
     * Sets the response to "INVALID COMMAND" if an unrecognized command is encountered.
     *
     * @param response The response string to be set to "INVALID COMMAND".
     */
    void HandleInvalidCommand(std::string &response);

    /**
     * @brief Handles invalid command formats.
     *
     * Sets the response to "INVALID COMMAND FORMAT" if the command format is incorrect.
     *
     * @param response The response string to be set to "INVALID COMMAND FORMAT".
     */
    void HandleInvalidCommandFormat(std::string &response);

    /**
     * @brief Handles invalid RESP types.
     *
     * Sets the response to "INVALID RESP TYPE" if an unknown RESP type is encountered.
     *
     * @param response The response string to be set to "INVALID RESP TYPE".
     */
    void HandleInvalidRespType(std::string &response);
};

#endif // MESSAGEPROCESSOR_H