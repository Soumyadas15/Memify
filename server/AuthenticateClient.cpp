#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Server.h"
#include "HMACUtil.h"

bool Server::AuthenticateClient(int client_fd)
{
    char buffer[1024] = {0};
    int valread = read(client_fd, buffer, 1024);
    if (valread <= 0)
    {
        return false;
    }

    std::string message(buffer);
    size_t delimiter_pos = message.find('|');
    if (delimiter_pos == std::string::npos)
    {
        return false;
    }

    std::string timestamp = message.substr(0, delimiter_pos);
    std::string signature = message.substr(delimiter_pos + 1);

    auto now = std::chrono::system_clock::now();
    auto timestamp_time = std::chrono::system_clock::from_time_t(std::stoll(timestamp));
    auto difference = std::chrono::duration_cast<std::chrono::minutes>(now - timestamp_time);
    if (std::abs(difference.count()) > 5)
    {
        return false;
    }

    if (VerifySignature(timestamp, signature))
    {
        const char *response = "OK\n";
        send(client_fd, response, strlen(response), 0);
        return true;
    }
    else
    {
        const char *response = "FAIL\n";
        send(client_fd, response, strlen(response), 0);
        return false;
    }
}