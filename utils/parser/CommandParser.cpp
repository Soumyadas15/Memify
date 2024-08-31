#include "CommandParser.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

// Parses an input string into an MESPObject based on its type.
// The input string is expected to start with a character that indicates the MESP type:
// '+' for SimpleString, '-' for Error, ':' for Integer, '$' for BulkString, and '*' for Array.
// If the input is empty or has an unknown type, exceptions are thrown.
//
// @param input A reference to the input string to be parsed.
// @return The parsed MESPObject.
// @throws std::runtime_error If the input is empty or has an unknown MESP type.
MESPObject CommandParser::parse(std::string &input)
{
    if (input.empty())
    {
        throw std::runtime_error("Empty input");
    }

    MESPObject parsedObj;

    switch (input[0])
    {
    case '+':
        parsedObj = parseSimpleString(input);
        break;
    case '-':
        parsedObj = parseError(input);
        break;
    case ':':
        parsedObj = parseInteger(input);
        break;
    case '#':
        parsedObj = parseFloat(input);
        break;
    case '$':
        parsedObj = parseBulkString(input);
        break;
    case '*':
        parsedObj = parseArray(input);
        break;
    default:
        throw std::runtime_error("Unknown MESP type");
    }

    return parsedObj;
}

// Serializes an MESPObject into a string format based on its type.
// Depending on the type of the MESPObject (SimpleString, Error, Integer, BulkString, or Array),
// the corresponding serialization method is called to convert the object to a string.
//
// @param obj The MESPObject to be serialized.
// @return A string representing the serialized MESPObject.
// @throws std::runtime_error If the MESPObject has an unknown type.
std::string CommandParser::serializeResponse(const MESPObject &obj)
{
    switch (obj.type)
    {
    case MESPType::SimpleString:
        return serializeSimpleString(obj.stringValue);
    case MESPType::Error:
        return serializeError(obj.stringValue);
    case MESPType::Integer:
        return serializeInteger(obj.intValue);
    case MESPType::Float:
        return serializeFloat(obj.floatValue);
    case MESPType::BulkString:
        return serializeBulkString(obj.stringValue);
    case MESPType::Array:
        return serializeArray(obj.arrayValue);
    default:
        throw std::runtime_error("Unknown MESP type");
    }
}

// Prints the details of an MESPObject to the standard output.
// The output format depends on the type of the MESPObject:
// SimpleString, Error, Integer, BulkString, or Array. For Array types, each element
// is printed recursively.
//
// @param obj The MESPObject to be printed.
void CommandParser::printMESPObject(const MESPObject &obj)
{
    switch (obj.type)
    {
    case MESPType::SimpleString:
        std::cout << "SimpleString: " << obj.stringValue << std::endl;
        break;
    case MESPType::Error:
        std::cout << "Error: " << obj.stringValue << std::endl;
        break;
    case MESPType::Integer:
        std::cout << "Integer: " << obj.intValue << std::endl;
        break;
    case MESPType::BulkString:
        std::cout << "BulkString: " << obj.stringValue << std::endl;
        break;
    case MESPType::Array:
        std::cout << "Array: [";
        for (const auto &elem : obj.arrayValue)
        {
            printMESPObject(elem); // Recursively print each element
        }
        std::cout << "]" << std::endl;
        break;
    default:
        std::cout << "Unknown MESP type" << std::endl;
    }
}