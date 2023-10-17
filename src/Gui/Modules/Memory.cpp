#include "Memory.h"

// void GuiDraw::Memory::RenderMemoryInfo(bool *keepOpen)
// {
//     ImGui::Begin("Memory", keepOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
//     ImGui::TextColored(ImVec4(1, 1, 0, 1), "Memory Information");
//     auto maxMemoryString = std::to_string(memInfo.GetTotal(Utility::SizeType::gb));
//     ImGui::Text("Total Memory: %s GB", maxMemoryString.c_str());
//     auto freeMemoryString = std::to_string(memInfo.GetFree(Utility::SizeType::gb));
//     ImGui::Text("Free Memory: %s GB", freeMemoryString.c_str());
//     auto availableMemoryString = std::to_string(memInfo.GetAvailable(Utility::SizeType::gb));
//     ImGui::Text("Available Memory: %s GB", availableMemoryString.c_str());

//     // #TODO: Add memory usage graph or pie chart
//     ImGui::End();
// }

void GuiDraw::Memory::RenderMemoryGraph(float text_y, float text_x, float graph_x, bool registerValue)
{
    ImGui::SetCursorPosX(text_x);

    // Convert the circular buffer to a vector for contiguous storage
    std::vector<float> values(buffer.begin(), buffer.end());

    // Move cursor to the calculated X position
    ImGui::SetCursorPosX(graph_x);
    ImGui::SetCursorPosY(text_y);

    ImGui::PlotLines((""),
                     values.data(), values.size(),
                     0, nullptr, 0, memInfo.GetTotal(Utility::SizeType::kb), {(100.0F), (50.0F)});
}

void GuiDraw::Memory::RenderMemoryInfo(bool *keepOpen, bool registerValue)
{
    // Variables
    const float plot_width = 100.0F;
    const float plot_height = 50.0F;
    const float padding = 5.0F;

    // Fill the entire ring buffer with zeroes, so that the graph won't resize
    if (buffer.size() == 0)
    {
        for (size_t i = 0; i < Settings::CpuBufferGraphSize; i++)
        {
            buffer.push_back(0);
        }
    }

    ImGui::Begin("Memory", keepOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    float window_width = ImGui::GetWindowWidth();

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Memory Information");
    auto cursor_y = ImGui::GetCursorPosY();
    auto maxMemoryString = std::to_string(memInfo.GetTotal(Utility::SizeType::gb));
    ImGui::Text("Total Memory: %s GB", maxMemoryString.c_str());
    auto freeMemoryString = std::to_string(memInfo.GetFree(Utility::SizeType::gb));
    ImGui::Text("Free Memory: %s GB", freeMemoryString.c_str());
    auto availableMemoryString = std::to_string(memInfo.GetAvailable(Utility::SizeType::gb));
    ImGui::Text("Available Memory: %s GB", availableMemoryString.c_str());

    if (registerValue)
    {
        buffer.push_back(memInfo.GetAvailable(Utility::SizeType::kb));
    }

    auto cursor_x = padding; // Grab cursor default position at the left of screen
    float right_aligned_x_pos = window_width - plot_width - ImGui::GetStyle().WindowPadding.x - padding * 3;

    RenderMemoryGraph(cursor_y, cursor_x, right_aligned_x_pos, registerValue);

    ImGui::End();
}
