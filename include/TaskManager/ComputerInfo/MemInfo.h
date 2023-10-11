#pragma once

#include <string>
#include "util.h"
#include <iostream>
#include <fstream>

using std::string;

class MemInfo
{
public:
    MemInfo()
    {
        Refresh();
    }

    void Refresh();
    int GetTotal(Utility::SizeType size = Utility::SizeType::kb);
    int GetFree(Utility::SizeType size = Utility::SizeType::kb);
    int GetAvailable(Utility::SizeType size = Utility::SizeType::kb);

private:
    string content;
};
