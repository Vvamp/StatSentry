#include "util.h"

string Utility::rtrim(const string &s)
{
    size_t end = s.find_last_not_of(" \t");
    if (end == string::npos)
    { // No non-space characters
        return "";
    }
    return s.substr(0, end + 1);
}

std::vector<std::unordered_map<string, string>> Utility::parseProcessorInfo(const string &content)
{
    std::vector<std::unordered_map<string, string>> processors;
    std::istringstream iss(content);

    string line;
    std::unordered_map<string, string> currentProcessor;

    while (std::getline(iss, line))
    {
        if (line.empty())
        {
            if (!currentProcessor.empty())
            {
                processors.push_back(currentProcessor);
                currentProcessor.clear();
            }
            continue;
        }

        string key, value;
        std::istringstream lineStream(line);
        std::getline(lineStream, key, ':');
        std::getline(lineStream, value);

        // Trim leading whitespaces from value
        value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch)
                                                { return !std::isspace(ch); }));
        value = Utility::rtrim(value);
        key = Utility::rtrim(key);

        currentProcessor[key] = value;
    }

    // Push the last processor if exists
    if (!currentProcessor.empty())
    {
        processors.push_back(currentProcessor);
    }

    return processors;
}

string Utility::getValueFromSysInfo(const string &content, const string &key)
{
    size_t index = content.find(key);

    if (index == string::npos)
    {
        // Key not found
        std::cout << "key not found\n";
        return "";
    }

    // Move to the end of the key to find the colon
    index += key.length();

    // Find the colon and skip it, and any additional whitespace
    index = content.find(":", index);
    if (index == string::npos)
    {
        // Colon not found after key, something's wrong
        std::cout << "colon not found\n";

        return "";
    }
    ++index; // Skip the colon

    // Skip any additional whitespace
    while (index < content.size() && std::isspace(content[index]))
    {
        ++index;
    }

    // Extract the value
    string value;
    while (index < content.size() && content[index] != '\n')
    {
        value += content[index];
        ++index;
    }

    return value;
}

std::vector<string> Utility::getValuesFromSysInfo(const string &content, const string &key)
{
    std::vector<string> values;
    size_t index = 0;

    while ((index = content.find(key, index)) != string::npos)
    {
        // Move to the end of the key to find the colon
        index += key.length();

        // Find the colon and skip it, and any additional whitespace
        size_t colonIndex = content.find(":", index);
        if (colonIndex == string::npos)
        {
            // Colon not found after key, something's wrong
            std::cout << "colon not found\n";
            return values;
        }
        index = colonIndex + 1; // Skip the colon

        // Skip any additional whitespace
        while (index < content.size() && std::isspace(content[index]))
        {
            ++index;
        }

        // Extract the value
        string value;
        while (index < content.size() && content[index] != '\n')
        {
            value += content[index];
            ++index;
        }

        values.push_back(value);
    }

    return values;
}

int Utility::countSubstrings(const string &str, const string &sub)
{
    int count = 0;
    std::size_t pos = 0; // To store the position of the found substring

    while ((pos = str.find(sub, pos)) != string::npos)
    {
        ++count;
        pos += sub.length(); // Move to the end of the found substring
    }

    return count;
}

int Utility::convertSizeFromKB(int val, SizeType size)
{
    switch (size)
    {
    default:
    case SizeType::kb:
    {
        return val;
    }

    case SizeType::mb:
    {
        return std::ceil(val / 1024);
    }
    case SizeType::gb:
    {
        return std::ceil(val / (1024.0 * 1024.0));
    }
    }
}