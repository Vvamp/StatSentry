#pragma once

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include "MemInfo.h"

namespace GuiDraw
{
    class Memory
    {
    public:
        Memory(MemInfo &memInfo) : memInfo(memInfo)
        {
        }
        void RenderMemoryInfo(bool *keepOpen);

    private:
        MemInfo &memInfo;
    };
};