#include "CommandParser.h"
#include <stdexcept>
#include <sstream>

// Parses an error message from the input string and returns an MESPObject representing the error.
// The input string is expected to start with a '-' character followed by the error message,
// and ends with "\r\n" (carriage return and newline). The method extracts the error message,
// then removes the parsed portion from the input string.
//
// @param input A reference to the input string containing the serialized MESP error message.
// @return An MESPObject of type Error, populated with the parsed error message.
// @throws std::runtime_error If the input is malformed or does not properly represent an error message.
MESPObject CommandParser::parseError(std::string &input)
{
    // Find the position of the end of the error message
    size_t end = input.find("\r\n");
    if (end == std::string::npos)
    {
        throw std::runtime_error("Malformed error");
    }

    // Extract the error message from the substring between '-' and "\r\n"
    MESPObject obj{MESPType::Error, input.substr(1, end - 1)};
    input.erase(0, end + 2); // Remove the parsed error message from the input

    return obj; // Return the populated MESPObject
}