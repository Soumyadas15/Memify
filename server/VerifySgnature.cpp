#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "Server.h"
#include "HMACUtil.h"

bool Server::VerifySignature(const std::string &data, const std::string &signature)
{
    return HMACUtil::verifySignature(data, signature, secret_key_);
}