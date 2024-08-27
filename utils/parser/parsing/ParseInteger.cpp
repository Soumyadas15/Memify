#include "CommandParser.h"
#include <stdexcept>
#include <sstream>

// Parses an integer from the input string and returns an MESPObject representing the integer.
// The input string is expected to start with a ':' character followed by the integer value,
// and ends with "\r\n" (carriage return and newline). The method extracts the integer value,
// then removes the parsed portion from the input string.
//
// @param input A reference to the input string containing the serialized MESP integer.
// @return An MESPObject of type Integer, populated with the parsed integer value.
// @throws std::runtime_error If the input is malformed or does not properly represent an integer.
MESPObject CommandParser::parseInteger(std::string &input)
{
    // Find the position of the end of the integer value
    size_t end = input.find("\r\n");
    if (end == std::string::npos)
    {
        throw std::runtime_error("Malformed integer");
    }

    // Extract the integer value from the substring between ':' and "\r\n"
    long long intValue = std::stoll(input.substr(1, end - 1));
    MESPObject obj{MESPType::Integer, intValue}; // Use the constructor for Integer type
    input.erase(0, end + 2); // Remove the parsed integer from the input

    return obj; // Return the populated MESPObject
}