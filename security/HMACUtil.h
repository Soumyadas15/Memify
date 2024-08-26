#ifndef HMACUTIL_H
#define HMACUTIL_H

#include <string>

class HMACUtil
{
public:
    static bool verifySignature(const std::string &data, const std::string &signature, const std::string &secret_key);
};

#endif // HMACUTIL_H