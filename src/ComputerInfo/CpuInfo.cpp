#include "CpuInfo.h"

void CpuInfo::Refresh()
{
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (cpuinfo.is_open())
    {
        std::stringstream buffer;
        buffer << cpuinfo.rdbuf();
        content = buffer.str(); // Store content as a member variable for later use
        cpuinfo.close();
    }
    else
    {
        // Handle error
        std::cout << "Error: failed to read cpuinfo.\n";
        return;
    }

    processors = Utility::parseProcessorInfo(content);
}

string CpuInfo::GetCpuName()
{
    return processors[0]["model name"];
}

int CpuInfo::GetPhysicalCores()
{

    try
    {
        return std::stoi(processors[0]["cpu cores"]);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1; // Return core count from cpuinfo
    }
}

int CpuInfo::GetLogicalCores()
{
    // return Utility::countSubstrings(content, "processor");
    return processors.size();
}

std::vector<std::unordered_map<std::string, std::string>> & CpuInfo::GetProcessors()
{
    return processors;
}

