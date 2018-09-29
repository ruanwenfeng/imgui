cmake_minimum_required(VERSION 3.11)

project(ImGuiDemo)
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)
file(GLOB IMGUI_SRC_LIST *.cpp)
file(GLOB IMGUI_PROJECT_HEADERS include/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(demo)

