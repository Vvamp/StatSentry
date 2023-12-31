# Set minimum CMake version and project name
cmake_minimum_required(VERSION 3.10)
project(StatSentry)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set compiler to Clang (optional)
set(CMAKE_CXX_COMPILER "clang++")

# OpenGL Preference
set(OpenGL_GL_PREFERENCE "GLVND")

# Debug (temp)
set(CMAKE_BUILD_TYPE Debug)


# Include directories
include_directories(include/)
include_directories(include/imgui)
include_directories(include/TaskManager/Util)
include_directories(include/TaskManager/Gui/Modules)
include_directories(include/TaskManager/ComputerInfo)

# Source files for ImGui
set(IMGUI_SOURCES
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_impl_opengl3.cpp
    external/imgui/imgui_impl_sdl2.cpp
)

# Source files for your project
set(STATSENTRY_SOURCES
    src/main.cpp
    src/ComputerInfo/ComputerInfo.cpp
    src/ComputerInfo/CpuInfo.cpp
    src/ComputerInfo/MemInfo.cpp
    src/Gui/Modules/CPU.cpp
    src/Gui/Modules/Memory.cpp
    src/Util/Util.cpp
    # Add other source files
)

# Create executable
add_executable(StatSentry ${STATSENTRY_SOURCES} ${IMGUI_SOURCES})

# Find and link SDL2 and GLEW
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED)
target_link_libraries(StatSentry PRIVATE ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${Boost_LIBRARIES})
