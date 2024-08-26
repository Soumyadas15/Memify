#include "HMACUtil.h"
#include <openssl/hmac.h>
#include <sstream>
#include <iomanip>

bool HMACUtil::verifySignature(const std::string &data, const std::string &signature, const std::string &secret_key)
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    HMAC(EVP_sha256(), secret_key.c_str(), secret_key.size(),
         reinterpret_cast<const unsigned char *>(data.c_str()), data.size(),
         hash, &hash_len);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return signature == ss.str();
}