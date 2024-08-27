#include "HMACUtil.h"
#include <openssl/hmac.h>
#include <sstream>
#include <iomanip>

/**
 * @brief Verifies an HMAC SHA-256 signature for a given data string.
 *
 * This method takes a data string, a provided HMAC signature, and a secret key,
 * then calculates the HMAC SHA-256 hash of the data using the secret key.
 * It compares the computed hash with the provided signature to verify the data's integrity and authenticity.
 *
 * @param data The original data string for which the HMAC signature was generated.
 * @param signature The provided HMAC signature that needs to be verified.
 * @param secret_key The secret key used for generating the HMAC signature.
 *
 * @return Returns true if the computed HMAC signature matches the provided signature, indicating valid and untampered data. Returns false otherwise.
 */
bool HMACUtil::verifySignature(const std::string &data, 
                               const std::string &signature, 
                               const std::string &secret_key
)
{
    // Buffer to store the generated hash
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    // Generate the HMAC SHA-256 hash using the provided secret key and data
    HMAC(EVP_sha256(),                                          // The hash function to use (SHA-256 in this case)
         secret_key.c_str(),                                    // Pointer to the secret key
         secret_key.size(),                                     // Length of the secret key
         reinterpret_cast<const unsigned char *>(data.c_str()), // Pointer to the data
         data.size(),                                           // Length of the data
         hash,                                                  // Buffer to store the result
         &hash_len);                                            // Variable to store the length of the result

    // Convert the binary hash to a hexadecimal string for comparison
    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    // Compare the generated hash with the provided signature
    return signature == ss.str();
}