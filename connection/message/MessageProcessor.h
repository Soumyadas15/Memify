#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <memory>
#include <string>
#include <sstream>
#include <ICache.h>

/**
 * @class MessageProcessor
 * @brief This class is responsible for processing messages and handling specific commands.
 * It uses a cache object to manage and retrieve data as needed.
 *
 * MessageProcessor provides a mechanism to interpret and respond to incoming messages.
 * It can parse commands and manage responses using a caching mechanism represented by the ICache interface.
 */
class MessageProcessor
{
public:
    /**
     * @brief Constructor for the MessageProcessor class.
     *
     * Initializes the MessageProcessor with a shared pointer to an ICache object.
     * This allows the class to interact with the cache for storing and retrieving data.
     *
     * @param cache A shared pointer to an ICache object used for caching purposes.
     */
    explicit MessageProcessor(std::shared_ptr<ICache> cache);

    /**
     * @brief Handles an incoming message and generates an appropriate response.
     *
     * This method interprets the provided message, processes any commands contained within it,
     * and constructs a response based on the message content and cache state.
     *
     * @param message The input message to be processed.
     * @param response The output response generated after processing the message.
     */
    void HandleMessage(const std::string &message, std::string &response);

private:
    std::shared_ptr<ICache> cache_; /**< A shared pointer to an ICache object used to manage cached data. */

    /**
     * @brief Processes a specific command from the input message.
     *
     * This method is responsible for handling a specific command parsed from the input message.
     * It uses a string stream to further parse command arguments and generate a response accordingly.
     *
     * @param command The command string extracted from the message.
     * @param iss A string stream containing the remainder of the message for further parsing.
     * @param response The response string that will be populated based on the command's execution.
     */
    void HandleCommand(const std::string &command, std::istringstream &iss, std::string &response);
};

#endif // MESSAGEPROCESSOR_H