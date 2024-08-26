#include "ResponseSender.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

void ResponseSender::Send(int client_fd, const std::string &response)
{
    uint32_t response_length = htonl(static_cast<uint32_t>(response.size()));
    send(client_fd, &response_length, sizeof(response_length), 0);
    send(client_fd, response.c_str(), response.size(), 0);
}