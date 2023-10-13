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

#include "ComputerInfo.h"
#include "CPU.h"
#include "Memory.h"
#include "Settings.h"

using std::ifstream;
using std::string;

enum class TaskPage
{
    CPU,
    Memory
};

int main(int, char **)
{

    // Initialize Data
    auto data = ComputerInfo();
    auto cpuGui = GuiDraw::Cpu(data.CPU);
    auto memGui = GuiDraw::Memory(data.Memory);

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
    SDL_Window *window = SDL_CreateWindow("StatSentry Task Manager", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
    int framesSinceLastPoll = 0;
    TaskPage task = TaskPage::CPU;
    while (run)
    {
        frameStart = SDL_GetTicks();                // Get ticks before rendering and logic
        SDL_GetWindowSize(window, &width, &height); // Get SDL window size
        if (framesSinceLastPoll > Settings::PollFrequencyFrameCount)
        {
            data.Refresh();
            framesSinceLastPoll = 0;
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

        // Button Panel
        int buttonPanelHeight = 200;
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(width, buttonPanelHeight), ImGuiCond_Always);
        ImGui::Begin("Buttons", &run, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        if (ImGui::Button("CPU")) // Buttons return true when clicked (most widgets return true when edited/activated)
            task = TaskPage::CPU;
        ImGui::SameLine();
        if (ImGui::Button("Memory")) // Buttons return true when clicked (most widgets return true when edited/activated)
            task = TaskPage::Memory;
        ImGui::SameLine();
        if (ImGui::Button("Disk")) // Buttons return true when clicked (most widgets return true when edited/activated)
            std::cout << "Disk pressed\n";
        ImGui::End();

        switch (task)
        {
        case TaskPage::CPU:
        {
            ImGui::SetNextWindowPos(ImVec2(0, buttonPanelHeight), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(width, height - buttonPanelHeight), ImGuiCond_Always);
            cpuGui.RenderCpuInfo(&run, (framesSinceLastPoll == 0));
            break;
        }
        case TaskPage::Memory:
        {
            ImGui::SetNextWindowPos(ImVec2(0, buttonPanelHeight), ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(width, height - buttonPanelHeight), ImGuiCond_Always);
            memGui.RenderMemoryInfo(&run);
            break;
        }
        }

        // Rendering
        ImGui::Render();
        SDL_GL_MakeCurrent(window, gl_context);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

        // pause render until 'Settings::FrameTime_Ms' ticks have passed
        frameTime = SDL_GetTicks() - frameStart; // Calculate the time taken for one loop
        if (Settings::FrameTime_Ms > frameTime)  // If the loop finished before the desired delay
        {
            SDL_Delay(Settings::FrameTime_Ms - frameTime); // Delay the loop to achieve the desired frame time
        }
        framesSinceLastPoll++;
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
