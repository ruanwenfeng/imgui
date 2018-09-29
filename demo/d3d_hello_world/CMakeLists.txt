include_directories(include)
include_directories(../dxut/include)
add_executable("D3DHelloWorld" main.cpp game_timer.cpp d3d_app.cpp include/game_timer.h include/d3d_util.h include/d3d_app.h)
target_link_libraries("D3DHelloWorld" DXUT d3d11.lib d3dcompiler.lib)