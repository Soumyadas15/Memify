#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

#include "ConnectionHandler.h"
#include "MessageProcessor.h"
#include "HMACUtil.h"
#include "LoggerManager.h"
#include "FileLogger.h"

ConnectionHandler::ConnectionHandler(std::shared_ptr<ICache> cache, 
                                        int client_fd, const std::string &secret_key
    )
    : cache_(std::move(cache)), client_fd_(client_fd), secret_key_(secret_key) 
    {
        std::ostringstream oss;
        oss << "connection_" << client_fd_ << ".log";

        file_logger_ = std::make_shared<FileLogger>(oss.str());
        file_logger_->setLogLevel(ILogger::LogLevel::DEBUG);

        LoggerManager::getInstance().addLogger(file_logger_);
        file_logger_->info("New connection started for client_fd: " + std::to_string(client_fd_));
    }

void ConnectionHandler::HandleClient()
{
    LoggerManager::getInstance().info("Client connected");
    std::vector<char> buffer;
    uint32_t expected_length = 0;

    while (true)
    {
        char chunk[1024];
        ssize_t bytes_received = recv(client_fd_, chunk, sizeof(chunk), 0);

        if (bytes_received <= 0)
        {
            if (bytes_received < 0)
            {
                LoggerManager::getInstance().error("Recv failed: " + std::string(strerror(errno)));
                file_logger_->error("Recv failed: " + std::string(strerror(errno)));
            }
            else
            {
                LoggerManager::getInstance().info("Client disconnected");
                file_logger_->info("Client disconnected");
            }
            break;
        }

        buffer.insert(buffer.end(), chunk, chunk + bytes_received);

        while (!buffer.empty())
        {
            if (expected_length == 0)
            {
                if (buffer.size() < 4)
                    break;
                expected_length = ntohl(*reinterpret_cast<uint32_t *>(buffer.data()));
                buffer.erase(buffer.begin(), buffer.begin() + 4);
            }

            if (buffer.size() < expected_length)
                break;

            std::string message(buffer.begin(), buffer.begin() + expected_length);
            buffer.erase(buffer.begin(), buffer.begin() + expected_length);
            expected_length = 0;

            size_t delimiter_pos = message.find('\n');
            if (delimiter_pos == std::string::npos)
            {
                LoggerManager::getInstance().error("Invalid message format: " + std::string(strerror(errno)));
                file_logger_->error("Invalid message format: " + std::string(strerror(errno)));
                SendResponse("Invalid message format");
                continue;
            }

            std::string signature = message.substr(0, delimiter_pos);
            std::string payload = message.substr(delimiter_pos + 1);

            if (verifySignature(payload, signature))
            {
                std::string response;
                ProcessMessage(payload, response);
                SendResponse(response);
            }
            else
            {
                LoggerManager::getInstance().error("Signature Verification Failure: " + std::string(strerror(errno)));
                file_logger_->error("Signature Verification Failure: " + std::string(strerror(errno)));
                SendResponse("Signature Yerification Failure");
            }
        }
    }

    close(client_fd_);
}

bool ConnectionHandler::verifySignature(const std::string &data, const std::string &signature)
{
    return HMACUtil::verifySignature(data, signature, secret_key_);
}

void ConnectionHandler::ProcessMessage(const std::string &message, std::string &response)
{
    MessageProcessor processor(cache_);
    processor.HandleMessage(message, response);
}

void ConnectionHandler::SendResponse(const std::string &response)
{
    uint32_t response_length = htonl(static_cast<uint32_t>(response.size()));
    send(client_fd_, &response_length, sizeof(response_length), 0);
    send(client_fd_, response.c_str(), response.size(), 0);
}