#pragma once

#include <string>
#include <vector>

// Enumeration defining different types of MESP objects
enum class MESPType
{
    SimpleString, // Represents a simple string value
    Error,        // Represents an error message
    Integer,      // Represents an integer value
    Float,        // Represents a float value
    BulkString,   // Represents a bulk string value
    Array         // Represents an array of MESP objects
};

// Structure representing an object in the MESP format
struct MESPObject
{
    MESPType type;                      // The type of the MESP object
    std::string stringValue;            // Holds the value for SimpleString, Error, or BulkString types
    long long intValue;                 // Holds the value for Integer type
    float floatValue;                   // Holds the value for Float type
    std::vector<MESPObject> arrayValue; // Holds the value for Array type, which is a vector of MESPObjects

    // Default constructor
    MESPObject()
        : type(MESPType::SimpleString), intValue(0), floatValue(0.0f) {}

    // Constructor for SimpleString, Error, and BulkString types
    MESPObject(MESPType t, const std::string &str)
        : type(t), stringValue(str), intValue(0), floatValue(0.0f) {}

    // Constructor for Integer type
    MESPObject(MESPType t, long long i)
        : type(t), intValue(i), floatValue(0.0f) {}

    // Constructor for Float type
    MESPObject(MESPType t, float f)
        : type(t), intValue(0), floatValue(f) {}

    // Constructor for Array type
    MESPObject(MESPType t, const std::vector<MESPObject> &arr)
        : type(t), arrayValue(arr), intValue(0), floatValue(0.0f) {}

    // Constructor for MESPType
    MESPObject(MESPType t)
        : type(t), intValue(0), floatValue(0.0f) {}
};

// Class responsible for parsing and serializing MESP objects
class CommandParser
{
public:
    /**
     * Parses a string input into an MESPObject.
     *
     * @param input A reference to the input string to be parsed.
     * @return The parsed MESPObject.
     */
    static MESPObject parse(std::string &input);

    /**
     * Serializes an MESPObject into a string response.
     *
     * @param obj The MESPObject to be serialized.
     * @return A string representing the serialized MESPObject.
     */
    static std::string serializeResponse(const MESPObject &obj);

    /**
     * Prints the details of an MESPObject to the standard output.
     *
     * @param obj The MESPObject to be printed.
     */
    static void printMESPObject(const MESPObject &obj);

private:
    /**
     * Parses a SimpleString type from the input string.
     *
     * @param input A reference to the input string to be parsed.
     * @return The parsed MESPObject of type SimpleString.
     */
    static MESPObject parseSimpleString(std::string &input);

    /**
     * Parses an Error type from the input string.
     *
     * @param input A reference to the input string to be parsed.
     * @return The parsed MESPObject of type Error.
     */
    static MESPObject parseError(std::string &input);

    /**
     * Parses an Integer type from the input string.
     *
     * @param input A reference to the input string to be parsed.
     * @return The parsed MESPObject of type Integer.
     */
    static MESPObject parseInteger(std::string &input);

    /**
     * Parses an Float type from the input string.
     *
     * @param input A reference to the input string to be parsed.
     * @return The parsed MESPObject of type Float.
     */
    static MESPObject parseFloat(std::string &input);

    /**
     * Parses a BulkString type from the input string.
     *
     * @param input A reference to the input string to be parsed.
     * @return The parsed MESPObject of type BulkString.
     */
    static MESPObject parseBulkString(std::string &input);

    /**
     * Parses an Array type from the input string.
     *
     * @param input A reference to the input string to be parsed.
     * @return The parsed MESPObject of type Array.
     */
    static MESPObject parseArray(std::string &input);

    /**
     * Serializes a SimpleString type into a string.
     *
     * @param str The string value to be serialized.
     * @return A string representing the serialized SimpleString.
     */
    static std::string serializeSimpleString(const std::string &str);

    /**
     * Serializes an Error type into a string.
     *
     * @param err The error message to be serialized.
     * @return A string representing the serialized Error.
     */
    static std::string serializeError(const std::string &err);

    /**
     * Serializes an Integer type into a string.
     *
     * @param num The integer value to be serialized.
     * @return A string representing the serialized Integer.
     */
    static std::string serializeInteger(long long num);

    /**
     * Serializes an Float type into a string.
     *
     * @param num The Float value to be serialized.
     * @return A string representing the serialized Float.
     */
    static std::string serializeFloat(float num);

    /**
     * Serializes a BulkString type into a string.
     *
     * @param str The bulk string value to be serialized.
     * @return A string representing the serialized BulkString.
     */
    static std::string serializeBulkString(const std::string &str);

    /**
     * Serializes an Array type into a string.
     *
     * @param arr The vector of MESPObjects to be serialized.
     * @return A string representing the serialized Array.
     */
    static std::string serializeArray(const std::vector<MESPObject> &arr);
};