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
 * @brief Processes the incoming message and generates a response.
 *
 * This method delegates the message processing to a `MessageProcessor` instance. The message processor is responsible for handling the business logic, interacting with the cache, and preparing the appropriate response.
 *
 * @param message The message payload that needs to be processed.
 * @param response A reference to a string where the processed response will be stored.
 */
void ConnectionHandler::ProcessMessage(const std::string &message, std::string &response)
{
    // Create a MessageProcessor instance to handle the message.
    MessageProcessor processor(cache_, geo_cache_, time_series_cache_);
    // Process the message and generate the response.
    processor.HandleMessage(message, response);
}