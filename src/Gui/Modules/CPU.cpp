#include "CPU.h"

void GuiDraw::RenderCpuInfo(CpuInfo &cpuInfo, bool *keepOpen, bool registerValue)
{
    ImGui::Begin("CPU", keepOpen, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "CPU Information");
    ImGui::Text("CPU: %s", cpuInfo.GetCpuName().c_str());
    auto physCoreString = std::to_string(cpuInfo.GetPhysicalCores());
    ImGui::Text("Physical Cores: %s", physCoreString.c_str());
    ImGui::SetItemTooltip("Actual Cores your processor has");
    auto logicCoreString = std::to_string(cpuInfo.GetLogicalCores());
    ImGui::Text("Logical Cores: %s", logicCoreString.c_str());
    ImGui::SetItemTooltip("Physical cores + any virtual cores your cpu might have");
    int processor_count = cpuInfo.GetProcessors().size();
    const int buffer_size = 20;
    static std::vector<boost::circular_buffer<float>> buffers(processor_count, boost::circular_buffer<float>(buffer_size));
    if(buffers[0].size() == 0){
        for(boost::circular_buffer<float> & b : buffers){
            for(size_t i = 0; i < buffer_size; i++){
                b.push_back(0);
            }
        }
    }



    // check max frequency(for graph max) at /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq . Replace cpu0 with each cpu index. src https://serverfault.com/questions/48108/how-to-determine-max-cpu-speed-of-a-processor-in-linux
    for (size_t processor_index = 0; processor_index < processor_count; processor_index++)
    {
        auto& buffer = buffers[processor_index];
        auto processor = cpuInfo.GetProcessors()[processor_index];
        float proc_speed = std::stof(processor["cpu MHz"]);
        ImGui::Text("Processor (%zu) speed: %s", processor_index + 1, std::to_string(proc_speed).c_str());
        
        if(registerValue)
            buffer.push_back(proc_speed);
        
        // Convert the circular buffer to a vector for contiguous storage
        std::vector<float> values(buffer.begin(), buffer.end());

            ImGui::PlotLines(("Processor " + std::to_string(processor_index + 1)).c_str(), 
                     values.data(), values.size(),
                     0, nullptr, 0, 6000, {(100.0F), (50.0F)});
    }

    // float samples[100];
    // for (int n = 0; n < 100; n++)
    //     samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    // ImGui::PlotLines("Samples", samples, 100);
    ImGui::End();
}
