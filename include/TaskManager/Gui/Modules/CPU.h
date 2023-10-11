#pragma once 

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include "CpuInfo.h"

namespace GuiDraw{
    void RenderCpuInfo(CpuInfo & cpuInfo, bool* keepOpen);
};