#include "MemInfo.h"

void MemInfo::Refresh()
{
    std::ifstream meminfo("/proc/meminfo");
    if (meminfo.is_open())
    {
        std::stringstream buffer;
        buffer << meminfo.rdbuf();
        content = buffer.str(); // Store content as a member variable for later use
        meminfo.close();
    }
    else
    {
        // Handle error
        std::cout << "Error: failed to read meminfo.\n";
        return;
    }
}

int MemInfo::GetTotal(Utility::SizeType size)
{
    auto value = Utility::getValueFromSysInfo(content, "MemTotal");

    try
    {
        int indexToRemove = value.find(" kB");
        value.erase(indexToRemove, 3);
        int kb = std::stoi(value);
        return Utility::convertSizeFromKB(kb, size);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1; // Return core count from cpuinfo
    }
}

int MemInfo::GetFree(Utility::SizeType size)
{
    auto value = Utility::getValueFromSysInfo(content, "MemFree");

    try
    {
        int indexToRemove = value.find(" kB");
        value.erase(indexToRemove, 3);
        int kb = std::stoi(value);
        return Utility::convertSizeFromKB(kb, size);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1; // Return core count from cpuinfo
    }
}
int MemInfo::GetAvailable(Utility::SizeType size)
{
    auto value = Utility::getValueFromSysInfo(content, "MemAvailable");

    try
    {
        int indexToRemove = value.find(" kB");
        value.erase(indexToRemove, 3);
        int kb = std::stoi(value);
        return Utility::convertSizeFromKB(kb, size);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return -1; // Return core count from cpuinfo
    }
}
