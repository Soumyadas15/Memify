#ifndef RESPONSESENDER_H
#define RESPONSESENDER_H

#include <string>

class ResponseSender {
public:
    static void Send(int client_fd, const std::string& response);
};

#endif // RESPONSESENDER_H