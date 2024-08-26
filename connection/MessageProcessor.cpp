#include "MessageProcessor.h"
#include <iostream>
#include <ICache.h>


MessageProcessor::MessageProcessor(std::shared_ptr<ICache> cache) : cache_(std::move(cache)) {}

void MessageProcessor::HandleMessage(const std::string& message, std::string& response) {
    std::istringstream iss(message);
    std::string command;
    iss >> command;

    HandleCommand(command, iss, response);
}

void MessageProcessor::HandleCommand(const std::string& command, std::istringstream& iss, std::string& response) {
    if(command == "PING")
    {
        try
        {
            response = "PONG";
        }
        catch (const std::exception &e)
        {
            response = "PONG FAILED";
            std::cerr << "Pong failed" << std::endl;
        }
    }
    else if (command == "SET") 
    {
        std::string key;
        iss >> key;

        std::string value;
        std::getline(iss >> std::ws, value);

        size_t last_space = value.find_last_of(' ');
        if (last_space != std::string::npos) 
        {
            std::string duration_str = value.substr(last_space + 1);
            value = value.substr(0, last_space);

            try 
            {
                int duration = std::stoi(duration_str);
                cache_->Set(key, value, std::chrono::seconds(duration));
                response = "OK";
            } 
            catch (const std::exception& e) 
            {
                response = "INVALID DURATION";
                std::cerr << "Invalid duration in SET command: " << duration_str << std::endl;
            }
        } 
        else 
        {
            response = "INVALID COMMAND FORMAT";
            std::cerr << "Invalid SET command format: " << value << std::endl;
        }
    } 
    else if (command == "GET") 
    {
        std::string key;
        iss >> key;

        std::string value;
        if (cache_->Get(key, value)) {
            response = value;
        } else {
            response = "NOT FOUND";
        }
    } 
    else if (command == "DELETE") 
    {
        std::string key;
        iss >> key;

        cache_->Delete(key);
        response = "OK";
    } 
    else 
    {
        response = "INVALID COMMAND";
    }
}
