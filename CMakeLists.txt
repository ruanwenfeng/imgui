cmake_minimum_required(VERSION 3.11)

project(ImGuiDemo)


file(GLOB IMGUI_SRC_LIST *.cpp)
file(GLOB IMGUI_PROJECT_HEADERS include/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include"})


link_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib")
add_subdirectory(demo)

