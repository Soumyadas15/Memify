#ifndef HMACUTIL_H
#define HMACUTIL_H

#include <string>

/**
 * @class HMACUtil
 * @brief A utility class for handling HMAC (Hash-based Message Authentication Code) operations.
 *
 * The HMACUtil class provides static methods for verifying digital signatures using HMAC.
 * This is commonly used in secure applications to ensure the integrity and authenticity of data.
 */
class HMACUtil
{
public:
    /**
     * @brief Verifies an HMAC signature for a given data string.
     *
     * This static method takes a data string, a provided HMAC signature, and a secret key,
     * then verifies if the provided signature matches the computed HMAC signature of the data.
     * This verification process ensures that the data has not been tampered with and confirms
     * that the data originates from a trusted source that holds the secret key.
     *
     * @param data A string containing the original data for which the HMAC signature was generated.
     * @param signature A string containing the provided HMAC signature to be verified against the computed signature.
     * @param secret_key A string containing the secret key used for generating and verifying the HMAC signature.
     *
     * @return A boolean value indicating whether the provided signature is valid. Returns true if the signature matches, otherwise false.
     */
    static bool verifySignature(const std::string &data, const std::string &signature, const std::string &secret_key);
};

#endif // HMACUTIL_H