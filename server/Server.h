#ifndef SERVER_H
#define SERVER_H

#include "ICache.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <atomic>

class Server
{
public:
    Server(std::shared_ptr<ICache> cache, uint16_t port);
    void Start();
    void Stop();

private:
    std::shared_ptr<ICache> cache_;
    uint16_t port_;
    std::string secret_key_;
    std::atomic<bool> running_;

    void HandleClient(int client_fd);
};

#endif // SERVER_H