#pragma once 

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include "CpuInfo.h"
#include <boost/circular_buffer.hpp>

namespace GuiDraw{
    void RenderCpuInfo(CpuInfo & cpuInfo, bool* keepOpen, bool registerValue = false);
};