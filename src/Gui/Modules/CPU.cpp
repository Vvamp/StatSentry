#include "CPU.h"

void GuiDraw::Cpu::RenderCoreGraph(size_t core, float text_x, float graph_x, bool registerValue)
{
    ImGui::SetCursorPosX(text_x);
    auto &buffer = buffers[core];
    auto processor = cpuInfo.GetProcessors()[core];
    auto cursor_y = ImGui::GetCursorPosY();
    float proc_speed = std::stof(processor["cpu MHz"]);
    float cache_size = std::stof(processor["cache size"]);
    ImGui::Text("| Core %zu:", core + 1);
    ImGui::Text("|- Core Speed: %s", std::to_string(proc_speed).c_str());
    ImGui::Text("|- Cache: %s kB", std::to_string(cache_size).c_str());

    if (registerValue)
        buffer.push_back(proc_speed);

    // Convert the circular buffer to a vector for contiguous storage
    std::vector<float> values(buffer.begin(), buffer.end());

    // Move cursor to the calculated X position
    ImGui::SetCursorPosX(graph_x);
    ImGui::SetCursorPosY(cursor_y);

    ImGui::PlotLines((""),
                     values.data(), values.size(),
                     0, nullptr, 0, 6000, {(100.0F), (50.0F)});
}

void GuiDraw::Cpu::RenderCpuInfo(bool *keepOpen, bool registerValue)
{
    // Variables
    const float plot_width = 100.0F;
    const float plot_height = 50.0F;
    const float padding = 5.0F;

    ImGui::Begin("CPU", keepOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    float window_width = ImGui::GetWindowWidth();
    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "CPU Information");
    ImGui::Text("CPU: %s", cpuInfo.GetCpuName().c_str());
    auto physCoreString = std::to_string(cpuInfo.GetPhysicalCores());
    ImGui::Text("Physical Cores: %s", physCoreString.c_str());
    ImGui::SetItemTooltip("Actual Cores your processor has");
    auto logicCoreString = std::to_string(cpuInfo.GetLogicalCores());
    ImGui::Text("Logical Cores: %s", logicCoreString.c_str());
    ImGui::SetItemTooltip("Physical cores + any virtual cores your cpu might have");

    // Fill the entire ring buffer with zeroes, so that the graph won't resize
    if (buffers[0].size() == 0)
    {
        for (boost::circular_buffer<float> &b : buffers)
        {
            for (size_t i = 0; i < Settings::CpuBufferGraphSize; i++)
            {
                b.push_back(0);
            }
        }
    }

    auto cursor_x = padding; // Grab cursor default position at the left of screen
    float right_aligned_x_pos = window_width - plot_width - ImGui::GetStyle().WindowPadding.x - padding * 3;

    // check max frequency(for graph max) at /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq . Replace cpu0 with each cpu index. src https://serverfault.com/questions/48108/how-to-determine-max-cpu-speed-of-a-processor-in-linux
    for (size_t processor_index = 0; processor_index < processor_count; processor_index++)
    {
        RenderCoreGraph(processor_index, cursor_x, right_aligned_x_pos, registerValue);
    }

    ImGui::End();
}
