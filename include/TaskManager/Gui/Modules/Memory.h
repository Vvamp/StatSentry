#pragma once 

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include "MemInfo.h"

namespace GuiDraw{
    void RenderMemoryInfo(MemInfo & memInfo, bool* keepOpen);;
};