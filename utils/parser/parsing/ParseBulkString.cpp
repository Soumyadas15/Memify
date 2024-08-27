#include "CommandParser.h"
#include <stdexcept>
#include <sstream>

// Parses a bulk string from the input string and returns an MESPObject representing the bulk string.
// The input string is expected to start with a '$' character followed by the length of the bulk string,
// and ends with "\r\n" (carriage return and newline). If the length is -1, the method represents a nil value.
// Otherwise, it extracts the bulk string value, then removes the parsed portion from the input string.
//
// @param input A reference to the input string containing the serialized MESP bulk string.
// @return An MESPObject of type BulkString, populated with the parsed string value, or "nil" if length is -1.
// @throws std::runtime_error If the input is malformed, too short, or does not properly represent a bulk string.
MESPObject CommandParser::parseBulkString(std::string &input)
{
    // Find the position of the end of the length specifier
    size_t lengthEnd = input.find("\r\n");
    if (lengthEnd == std::string::npos)
    {
        throw std::runtime_error("Malformed bulk string");
    }

    // Extract the length of the bulk string from the substring between '$' and "\r\n"
    int length = std::stoi(input.substr(1, lengthEnd - 1));

    // Handle the special case of a nil bulk string
    if (length == -1)
    {
        MESPObject obj{MESPType::BulkString, "nil"};
        input.erase(0, lengthEnd + 2); // Remove the length specifier and "\r\n"
        return obj;
    }

    // Ensure the input is long enough to contain the bulk string and its ending "\r\n"
    size_t start = lengthEnd + 2;
    if (input.length() < start + length + 2)
    {
        throw std::runtime_error("Bulk string too short");
    }

    // Extract the bulk string value from the input
    MESPObject obj{MESPType::BulkString, input.substr(start, length)};
    input.erase(0, start + length + 2); // Remove the bulk string and its ending "\r\n"

    return obj; // Return the populated MESPObject
}