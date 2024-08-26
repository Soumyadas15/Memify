#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <memory>
#include <string>
#include <vector>
#include <sstream>

#include "FileLogger.h"

class ICache;

class ConnectionHandler
{
public:
    ConnectionHandler(std::shared_ptr<ICache> cache, int client_fd, const std::string &secret_key);
    void HandleClient();

private:
    std::shared_ptr<ICache> cache_;
    int client_fd_;
    std::string secret_key_;
    std::shared_ptr<FileLogger> file_logger_;

    bool verifySignature(const std::string &data, const std::string &signature);
    void ProcessMessage(const std::string &message, std::string &response);
    void SendResponse(const std::string &response);
};

#endif // CONNECTIONHANDLER_H