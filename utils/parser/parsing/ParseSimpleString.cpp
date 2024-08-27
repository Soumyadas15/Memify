#include "CommandParser.h"
#include <stdexcept>
#include <sstream>

// Parses a simple string from the input string and returns an MESPObject representing the simple string.
// The input string is expected to start with a '+' character followed by the string value,
// and ends with "\r\n" (carriage return and newline). The method extracts the string value,
// then removes the parsed portion from the input string.
//
// @param input A reference to the input string containing the serialized MESP simple string.
// @return An MESPObject of type SimpleString, populated with the parsed string value.
// @throws std::runtime_error If the input is malformed or does not properly represent a simple string.
MESPObject CommandParser::parseSimpleString(std::string &input)
{
    // Find the position of the end of the string value
    size_t end = input.find("\r\n");
    if (end == std::string::npos)
    {
        throw std::runtime_error("Malformed simple string");
    }

    // Extract the string value from the substring between '+' and "\r\n"
    MESPObject obj{MESPType::SimpleString, input.substr(1, end - 1)};
    input.erase(0, end + 2); // Remove the parsed simple string from the input

    return obj; // Return the populated MESPObject
}