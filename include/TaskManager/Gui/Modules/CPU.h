#pragma once

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <string>
#include "CpuInfo.h"
#include <boost/circular_buffer.hpp>
#include "Settings.h"

namespace GuiDraw
{
    class Cpu
    {
    public:
        Cpu(CpuInfo &cpuInfo) : cpuInfo(cpuInfo)
        {
            processor_count = cpuInfo.GetProcessors().size();
            buffers = std::vector<boost::circular_buffer<float>>(processor_count, boost::circular_buffer<float>(Settings::CpuBufferGraphSize));
        }

        void RenderCpuInfo(bool *keepOpen, bool registerValue = false);
        void RenderCoreGraph(size_t core, float text_x, float graph_x, bool registerValue);

    private:
        std::vector<boost::circular_buffer<float>> buffers;
        CpuInfo &cpuInfo;
        int processor_count = cpuInfo.GetProcessors().size();
    };
};