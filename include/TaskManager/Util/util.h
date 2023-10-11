#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

using std::string;

namespace Utility
{

    /// @brief Computer sizes
    enum class SizeType
    {
        gb,
        mb,
        kb
    };

    /// @brief Trims tabs and spaces from the end of a string
    /// @param s string to trim
    /// @return trimmed string
    string rtrim(const string &s);

    /// @brief Parses all processors from a cpuinfo string
    /// @param content String in the format of /proc/cpuinfo
    /// @return A list of values and keys of each processor
    std::vector<std::unordered_map<string, string>> parseProcessorInfo(const string &content);

    /// @brief Grab the first value based on a given key from a /proc info file(like cpuinfo or meminfo)
    /// @param content The info string
    /// @param key The key to grab the value of
    /// @return The appropriate key, or an empty string if not found
    string getValueFromSysInfo(const string &content, const string &key);

    /// @brief Grab all values based on a given key from a /proc info file(like cpuinfo or meminfo)
    /// @param content The info string
    /// @param key The key to grab the values of
    /// @return All matched values
    std::vector<string> getValuesFromSysInfo(const string &content, const string &key);

    /// @brief Count the occurence of a substring in a given string
    /// @param str The string that contains the substrings
    /// @param sub The substring to count
    /// @return The amount of times the substring occurs
    int countSubstrings(const string &str, const string &sub);

    /// @brief Given a SizeType, converts a value to that size(from kb)
    /// @param val A value in kb
    /// @param size The size to convert to
    /// @return The converted value
    int convertSizeFromKB(int val, SizeType size);
};