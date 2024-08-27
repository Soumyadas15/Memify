#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <memory>

#include "ConnectionHandler.h"
#include "MessageProcessor.h"
#include "HMACUtil.h"


/**
 * @brief Verifies the signature of the given payload using HMAC.
 *
 * This method checks whether the provided signature matches the HMAC of the payload data using the stored secret key. This ensures that the payload has not been tampered with during transmission.
 *
 * @param data The payload data whose signature needs to be verified.
 * @param signature The signature to verify against the payload data.
 * @return True if the signature matches the HMAC of the payload; otherwise, false.
 */
bool ConnectionHandler::VerifySignature(const std::string &data, const std::string &signature)
{
    // Use HMAC utility to verify the signature with the provided secret key.
    return HMACUtil::verifySignature(data, signature, secret_key_);
}