#include "CommandParser.h"
#include <stdexcept>
#include <sstream>

// Parses a float from the input string and returns an MESPObject representing the float.
// The input string is expected to start with a ':' character followed by the float value,
// and ends with "\r\n" (carriage return and newline). The method extracts the float value,
// then removes the parsed portion from the input string.
//
// @param input A reference to the input string containing the serialized MESP float.
// @return An MESPObject of type float, populated with the parsed float value.
// @throws std::runtime_error If the input is malformed or does not properly represent an float.
MESPObject CommandParser::parseFloat(std::string &input)
{
    // Find the position of the end of the float value
    size_t end = input.find("\r\n");
    if (end == std::string::npos)
    {
        throw std::runtime_error("Malformed float");
    }

    // Extract the float value from the substring between ':' and "\r\n"
    float value = std::stof(input.substr(1, end - 1));
    MESPObject obj{MESPType::Float, value}; // Use floatValue field

    input.erase(0, end + 2); // Remove the parsed float from the input

    return obj; // Return the populated MESPObject
}