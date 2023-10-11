#include "CPU.h"

void GuiDraw::RenderCpuInfo(CpuInfo & cpuInfo, bool* keepOpen)
{
    ImGui::Begin("CPU", keepOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "CPU Information");
    ImGui::Text("CPU: %s", cpuInfo.GetCpuName().c_str());
    auto physCoreString = std::to_string(cpuInfo.GetPhysicalCores());
    ImGui::Text("Physical Cores: %s", physCoreString.c_str());
    auto logicCoreString = std::to_string(cpuInfo.GetLogicalCores());
    ImGui::Text("Logical Cores: %s", logicCoreString.c_str());

    for (size_t processor_index = 0; processor_index < cpuInfo.GetProcessors().size(); processor_index++)
    {
        auto processor = cpuInfo.GetProcessors()[processor_index];
        ImGui::Text("Processor (%zu) speed: %s", processor_index + 1, processor["cpu MHz"].c_str());
    }

    ImGui::End();
}
