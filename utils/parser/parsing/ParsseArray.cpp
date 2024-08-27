#include "CommandParser.h"
#include <stdexcept>
#include <sstream>

// Parses an array from the input string and returns an MESPObject representing the array.
// The input string is expected to start with a '*' character followed by the number of elements in the array,
// and ends with "\r\n" (carriage return and newline). The method extracts the length of the array,
// then iteratively parses each element according to the MESP format until the array is fully constructed.
//
// @param input A reference to the input string containing the serialized MESP array.
// @return An MESPObject of type Array, populated with the parsed elements.
// @throws std::runtime_error If the input is malformed or does not properly represent an array.
MESPObject CommandParser::parseArray(std::string &input)
{
    // Find the position of the end of the length specifier
    size_t lengthEnd = input.find("\r\n");
    if (lengthEnd == std::string::npos)
    {
        throw std::runtime_error("Malformed array");
    }

    // Extract the length of the array from the substring before "\r\n"
    int length = std::stoi(input.substr(1, lengthEnd - 1));
    input.erase(0, lengthEnd + 2); // Remove the length specifier from the input

    MESPObject obj{MESPType::Array}; // Initialize MESPObject of type Array

    // Parse each element of the array
    for (int i = 0; i < length; ++i)
    {
        obj.arrayValue.push_back(parse(input)); // Parse and add each element to the array
    }
    return obj; // Return the populated MESPObject
}
