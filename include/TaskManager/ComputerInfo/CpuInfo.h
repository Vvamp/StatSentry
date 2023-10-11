#pragma once


#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include "util.h"

using std::string;

/// @brief A wrapper class containing the current system's cpu info
class CpuInfo
{
public:
    CpuInfo()
    {
        Refresh();
    }

    void Refresh();

    string GetCpuName();

    int GetPhysicalCores();
    int GetLogicalCores();

    std::vector<std::unordered_map<std::string, std::string>> &GetProcessors();

private:
    string content;
    std::vector<std::unordered_map<std::string, std::string>> processors;
};