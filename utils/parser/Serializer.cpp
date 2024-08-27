#include "CommandParser.h"
#include <sstream>

// Serializes a SimpleString type into a string format.
// The SimpleString format starts with a '+' character followed by the string value,
// and ends with "\r\n" (carriage return and newline).
//
// @param str The string value to be serialized.
// @return A string in the MESP SimpleString format.
std::string CommandParser::serializeSimpleString(const std::string &str)
{
    return "+" + str + "\r\n";
}

// Serializes an Error type into a string format.
// The Error format starts with a '-' character followed by the error message,
// and ends with "\r\n" (carriage return and newline).
//
// @param err The error message to be serialized.
// @return A string in the MESP Error format.
std::string CommandParser::serializeError(const std::string &err)
{
    return "-" + err + "\r\n";
}

// Serializes an Integer type into a string format.
// The Integer format starts with a ':' character followed by the integer value,
// and ends with "\r\n" (carriage return and newline).
//
// @param num The integer value to be serialized.
// @return A string in the MESP Integer format.
std::string CommandParser::serializeInteger(long long num)
{
    return ":" + std::to_string(num) + "\r\n";
}

// Serializes an Float type into a string format.
// The Float format starts with a ':' character followed by the Float value,
// and ends with "\r\n" (carriage return and newline).
//
// @param num The Float value to be serialized.
// @return A string in the MESP Float format.
std::string CommandParser::serializeFloat(float num)
{
    return "#" + std::to_string(num) + "\r\n";
}

// Serializes a BulkString type into a string format.
// The BulkString format starts with a '$' character followed by the length of the string,
// and ends with "\r\n" (carriage return and newline). If the string is "nil", it represents a nil bulk string.
//
// @param str The bulk string value to be serialized.
// @return A string in the MESP BulkString format. Returns "$-1\r\n" for nil values.
std::string CommandParser::serializeBulkString(const std::string &str)
{
    if (str == "nil")
    {
        return "$-1\r\n";
    }
    return "$" + std::to_string(str.length()) + "\r\n" + str + "\r\n";
}

// Serializes an Array type into a string format.
// The Array format starts with a '*' character followed by the number of elements in the array,
// and ends with "\r\n" (carriage return and newline). Each element in the array is serialized using
// the serializeResponse method to ensure proper formatting of nested MESP objects.
//
// @param arr The vector of MESPObjects to be serialized.
// @return A string in the MESP Array format.
std::string CommandParser::serializeArray(const std::vector<MESPObject> &arr)
{
    std::string result = "*" + std::to_string(arr.size()) + "\r\n";
    for (const auto &item : arr)
    {
        result += serializeResponse(item);
    }
    return result;
}