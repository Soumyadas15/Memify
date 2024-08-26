#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <memory>
#include <string>
#include <sstream>
#include <ICache.h>

class MessageProcessor
{
public:
    explicit MessageProcessor(std::shared_ptr<ICache> cache);
    void HandleMessage(const std::string &message, std::string &response);

private:
    std::shared_ptr<ICache> cache_;
    void HandleCommand(const std::string &command, std::istringstream &iss, std::string &response);
};

#endif // MESSAGEPROCESSOR_H
