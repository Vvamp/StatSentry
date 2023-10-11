#pragma once

#include "CpuInfo.h"
#include "MemInfo.h"

class ComputerInfo
{
public:
    CpuInfo CPU = CpuInfo();
    MemInfo Memory = MemInfo();

    void Refresh();
};