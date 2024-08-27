#include "MessageProcessor.h"
#include <iostream>
#include <ICache.h>

/**
 * @brief Constructs a MessageProcessor object with a given cache.
 *
 * The constructor initializes the MessageProcessor with a shared pointer to an ICache object.
 * This allows the MessageProcessor to interact with the cache for storing, retrieving, and deleting data.
 *
 * @param cache A shared pointer to an ICache object used for caching data.
 */
MessageProcessor::MessageProcessor(std::shared_ptr<ICache> cache) : cache_(std::move(cache)) {}

/**
 * @brief Handles an incoming message and generates an appropriate response.
 *
 * This method extracts a command from the input message and processes it accordingly.
 * It uses a string stream to parse the message and delegates command handling to the `HandleCommand` method.
 *
 * @param message The input message to be processed, expected to contain a command and arguments.
 * @param response The output response generated after processing the message.
 */
void MessageProcessor::HandleMessage(const std::string &message, std::string &response)
{
    std::istringstream iss(message); // Create a string stream to parse the message
    std::string command;
    iss >> command; // Extract the first word as the command

    // Pass the command and the rest of the message to the command handler
    HandleCommand(command, iss, response);
}

/**
 * @brief Processes a specific command extracted from the input message.
 *
 * This method is responsible for handling specific commands, such as PING, SET, GET, and DELETE.
 * It parses the command arguments using the provided string stream and interacts with the cache as needed.
 *
 * @param command The command string extracted from the message.
 * @param iss A string stream containing the remainder of the message for further parsing.
 * @param response The response string that will be populated based on the command's execution.
 */
void MessageProcessor::HandleCommand(const std::string &command, std::istringstream &iss, std::string &response)
{
    if (command == "PING")
    {
        // Handle PING command: Respond with "PONG"
        try
        {
            response = "PONG";
        }
        catch (const std::exception &e)
        {
            // If an error occurs, respond with "PONG FAILED"
            response = "PONG FAILED";
            std::cerr << "Pong failed" << std::endl;
        }
    }
    else if (command == "SET")
    {
        // Handle SET command: Expect a key, value, and optional duration
        std::string key;
        iss >> key; // Extract the key

        std::string value;
        std::getline(iss >> std::ws, value); // Extract the value, considering whitespace

        // Attempt to find a duration parameter at the end of the value string
        size_t last_space = value.find_last_of(' ');
        if (last_space != std::string::npos)
        {
            // Extract duration from the end of the value string
            std::string duration_str = value.substr(last_space + 1);
            value = value.substr(0, last_space);

            try
            {
                int duration = std::stoi(duration_str);                  // Convert duration string to integer
                cache_->Set(key, value, std::chrono::seconds(duration)); // Store in cache
                response = "OK";
            }
            catch (const std::exception &e)
            {
                // Respond with an error message if the duration is invalid
                response = "INVALID DURATION";
                std::cerr << "Invalid duration in SET command: " << duration_str << std::endl;
            }
        }
        else
        {
            // Respond with an error if the command format is incorrect
            response = "INVALID COMMAND FORMAT";
            std::cerr << "Invalid SET command format: " << value << std::endl;
        }
    }
    else if (command == "GET")
    {
        // Handle GET command: Expect a key to retrieve its value
        std::string key;
        iss >> key;

        std::string value;
        if (cache_->Get(key, value))
        {
            response = value; // Return the value if found in cache
        }
        else
        {
            response = "NOT FOUND"; // Return "NOT FOUND" if key does not exist in cache
        }
    }
    else if (command == "DELETE")
    {
        // Handle DELETE command: Expect a key to delete from cache
        std::string key;
        iss >> key;

        cache_->Delete(key); // Delete the key-value pair from cache
        response = "OK";     // Acknowledge successful deletion
    }
    else
    {
        // Handle any unrecognized command
        response = "INVALID COMMAND";
    }
}