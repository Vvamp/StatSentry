#include <SDL.h>
#include <GL/glew.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <sstream> // For stringstream
#include <cctype>
#include <vector>
#include <unordered_map>
#include <algorithm>

using std::ifstream;
using std::string;
enum class SizeType
{
    gb,
    mb,
    kb
};
namespace Helpers
{
    std::string rtrim(const std::string &s)
    {
        size_t end = s.find_last_not_of(" \t");
        if (end == std::string::npos)
        { // No non-space characters
            return "";
        }
        return s.substr(0, end + 1);
    }
    std::vector<std::unordered_map<std::string, std::string>> parseProcessorInfo(const std::string &content)
    {
        std::vector<std::unordered_map<std::string, std::string>> processors;
        std::istringstream iss(content);

        std::string line;
        std::unordered_map<std::string, std::string> currentProcessor;

        while (std::getline(iss, line))
        {
            if (line.empty())
            {
                if (!currentProcessor.empty())
                {
                    processors.push_back(currentProcessor);
                    currentProcessor.clear();
                }
                continue;
            }

            std::string key, value;
            std::istringstream lineStream(line);
            std::getline(lineStream, key, ':');
            std::getline(lineStream, value);

            // Trim leading whitespaces from value
            value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch)
                                                    { return !std::isspace(ch); }));
            value = Helpers::rtrim(value);
            key = Helpers::rtrim(key);

            currentProcessor[key] = value;
        }

        // Push the last processor if exists
        if (!currentProcessor.empty())
        {
            processors.push_back(currentProcessor);
        }

        return processors;
    }

    std::string getValueFromSysInfo(const std::string &content, const std::string &key)
    {
        size_t index = content.find(key);

        if (index == std::string::npos)
        {
            // Key not found
            std::cout << "key not found\n";
            return "";
        }

        // Move to the end of the key to find the colon
        index += key.length();

        // Find the colon and skip it, and any additional whitespace
        index = content.find(":", index);
        if (index == std::string::npos)
        {
            // Colon not found after key, something's wrong
            std::cout << "colon not found\n";

            return "";
        }
        ++index; // Skip the colon

        // Skip any additional whitespace
        while (index < content.size() && std::isspace(content[index]))
        {
            ++index;
        }

        // Extract the value
        std::string value;
        while (index < content.size() && content[index] != '\n')
        {
            value += content[index];
            ++index;
        }

        return value;
    }

    std::vector<std::string> getValuesFromSysInfo(const std::string &content, const std::string &key)
    {
        std::vector<std::string> values;
        size_t index = 0;

        while ((index = content.find(key, index)) != std::string::npos)
        {
            // Move to the end of the key to find the colon
            index += key.length();

            // Find the colon and skip it, and any additional whitespace
            size_t colonIndex = content.find(":", index);
            if (colonIndex == std::string::npos)
            {
                // Colon not found after key, something's wrong
                std::cout << "colon not found\n";
                return values;
            }
            index = colonIndex + 1; // Skip the colon

            // Skip any additional whitespace
            while (index < content.size() && std::isspace(content[index]))
            {
                ++index;
            }

            // Extract the value
            std::string value;
            while (index < content.size() && content[index] != '\n')
            {
                value += content[index];
                ++index;
            }

            values.push_back(value);
        }

        return values;
    }

    int countSubstrings(const std::string &str, const std::string &sub)
    {
        int count = 0;
        std::size_t pos = 0; // To store the position of the found substring

        while ((pos = str.find(sub, pos)) != std::string::npos)
        {
            ++count;
            pos += sub.length(); // Move to the end of the found substring
        }

        return count;
    }



    int convertSizeFromKB(int val, SizeType size){
         switch (size)
            {
                default:
                case SizeType::kb:
                {
                    return val;
                }

                case SizeType::mb:
                {
                    return std::ceil(val / 1024);
                }
                case SizeType::gb:
                {
                    return std::ceil(val / (1024.0 * 1024.0));
                }
            }
    }
};

class CpuInfo
{
public:
    CpuInfo()
    {
        Refresh();
    }

    void Refresh()
    {
        std::ifstream cpuinfo("/proc/cpuinfo");
        if (cpuinfo.is_open())
        {
            std::stringstream buffer;
            buffer << cpuinfo.rdbuf();
            content = buffer.str(); // Store content as a member variable for later use
            cpuinfo.close();
        }
        else
        {
            // Handle error
            std::cout << "Error: failed to read cpuinfo.\n";
            return;
        }

        processors = Helpers::parseProcessorInfo(content);
    }

    string GetCpuName()
    {
        return processors[0]["model name"];
    }

    int GetPhysicalCores()
    {

        try
        {
            return std::stoi(processors[0]["cpu cores"]);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return -1; // Return core count from cpuinfo
        }
    }

    int GetLogicalCores()
    {
        // return Helpers::countSubstrings(content, "processor");
        return processors.size();
    }

    std::vector<std::unordered_map<std::string, std::string>> &GetProcessors()
    {
        return processors;
    }

private:
    string content;
    std::vector<std::unordered_map<std::string, std::string>> processors;
};



class MemInfo
{
public:
    MemInfo()
    {
        Refresh();
    }

    void Refresh()
    {
        std::ifstream meminfo("/proc/meminfo");
        if (meminfo.is_open())
        {
            std::stringstream buffer;
            buffer << meminfo.rdbuf();
            content = buffer.str(); // Store content as a member variable for later use
            meminfo.close();
        }
        else
        {
            // Handle error
            std::cout << "Error: failed to read meminfo.\n";
            return;
        }
    }

    int GetTotal(SizeType size = SizeType::kb)
    {
        auto value = Helpers::getValueFromSysInfo(content, "MemTotal");

        try
        {
            int indexToRemove = value.find(" kB");
            value.erase(indexToRemove, 3);
            int kb = std::stoi(value);
            return Helpers::convertSizeFromKB(kb, size);
           
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return -1; // Return core count from cpuinfo
        }
    }

    int GetFree(SizeType size = SizeType::kb)
    {
        auto value = Helpers::getValueFromSysInfo(content, "MemFree");

        try
        {
            int indexToRemove = value.find(" kB");
            value.erase(indexToRemove, 3);
            int kb = std::stoi(value);
            return Helpers::convertSizeFromKB(kb, size);
           
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return -1; // Return core count from cpuinfo
        }
    }
    int GetAvailable(SizeType size = SizeType::kb)
    {
        auto value = Helpers::getValueFromSysInfo(content, "MemAvailable");

        try
        {
            int indexToRemove = value.find(" kB");
            value.erase(indexToRemove, 3);
            int kb = std::stoi(value);
            return Helpers::convertSizeFromKB(kb, size);
           
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            return -1; // Return core count from cpuinfo
        }
    }

private:
    string content;
};

class ComputerInfo
{
public:
    CpuInfo CPU = CpuInfo();
    MemInfo Memory = MemInfo();

    void Refresh()
    {
        CPU.Refresh();
        Memory.Refresh();
    }
};

int main(int, char **)
{

    // Initialize Data
    auto data = ComputerInfo();
    const Uint32 frameDelay = 100;    // 100 ms delay
    const int frameRefreshCount = 10; // how many frames to wait before refreshing data
    Uint32 frameStart;
    Uint32 frameTime;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        std::cout << "Error: " << SDL_GetError() << "\n";
        return -1;
    }

    // Initialize OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Create SDL window
    SDL_Window *window = SDL_CreateWindow("OpenTasks Task Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    // Initialize GLEW
    glewInit();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

    int width, height;
    // Main loop
    bool run = true;
    int frameCount = 0;
    while (run)
    {
        frameStart = SDL_GetTicks();                // Get ticks before rendering and logic
        SDL_GetWindowSize(window, &width, &height); // Get SDL window size
        if (frameCount > frameRefreshCount)
        {
            data.Refresh();
            frameCount = 0;
        }
        // Poll events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                run = false;
        }

        // Start ImGui Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, height / 2), ImGuiCond_Always);
        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("CPU", &run, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "CPU Information");
        ImGui::Text("CPU: %s", data.CPU.GetCpuName().c_str());
        auto physCoreString = std::to_string(data.CPU.GetPhysicalCores());
        ImGui::Text("Physical Cores: %s", physCoreString.c_str());
        auto logicCoreString = std::to_string(data.CPU.GetLogicalCores());
        ImGui::Text("Logical Cores: %s", logicCoreString.c_str());

        for (size_t processor_index = 0; processor_index < data.CPU.GetProcessors().size(); processor_index++)
        {
            auto processor = data.CPU.GetProcessors()[processor_index];
            ImGui::Text("Processor (%zu) speed: %s", processor_index + 1, processor["cpu MHz"].c_str());
        }

        ImGui::End();
        ImGui::SetNextWindowPos(ImVec2(0, height / 2), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, height / 2), ImGuiCond_Always);
        ImGui::Begin("Memory", &run, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Memory Information");
        auto maxMemoryString = std::to_string(data.Memory.GetTotal(SizeType::gb));
        ImGui::Text("Total Memory: %s GB", maxMemoryString.c_str());
        auto freeMemoryString = std::to_string(data.Memory.GetFree(SizeType::gb));
        ImGui::Text("Free Memory: %s GB", freeMemoryString.c_str());
        auto availableMemoryString = std::to_string(data.Memory.GetAvailable(SizeType::gb));
        ImGui::Text("Available Memory: %s GB", availableMemoryString.c_str());

        ImGui::End();

        // Rendering
        ImGui::Render();
        SDL_GL_MakeCurrent(window, gl_context);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

        // pause render until 'frameDelay' ticks have passed
        frameTime = SDL_GetTicks() - frameStart; // Calculate the time taken for one loop
        if (frameDelay > frameTime)              // If the loop finished before the desired delay
        {
            SDL_Delay(frameDelay - frameTime); // Delay the loop to achieve the desired frame time
        }
        frameCount++;
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
