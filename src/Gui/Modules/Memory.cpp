#include "Memory.h"

void GuiDraw::Memory::RenderMemoryInfo(bool *keepOpen)
{
    ImGui::Begin("Memory", keepOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Memory Information");
    auto maxMemoryString = std::to_string(memInfo.GetTotal(Utility::SizeType::gb));
    ImGui::Text("Total Memory: %s GB", maxMemoryString.c_str());
    auto freeMemoryString = std::to_string(memInfo.GetFree(Utility::SizeType::gb));
    ImGui::Text("Free Memory: %s GB", freeMemoryString.c_str());
    auto availableMemoryString = std::to_string(memInfo.GetAvailable(Utility::SizeType::gb));
    ImGui::Text("Available Memory: %s GB", availableMemoryString.c_str());

    ImGui::End();
}
