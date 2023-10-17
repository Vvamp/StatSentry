#pragma once

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include "MemInfo.h"
#include "Settings.h"
#include <boost/circular_buffer.hpp>
#include <iostream>

namespace GuiDraw
{
    class Memory
    {
    public:
        Memory(MemInfo &memInfo) : memInfo(memInfo), buffer(100)
        {}

        void RenderMemoryInfo(bool *keepOpen, bool registerValue);
        void RenderMemoryGraph(float text_y, float text_x, float graph_x, bool registerValue);
        
    private:
        MemInfo &memInfo;
        boost::circular_buffer<float> buffer;
    };
};