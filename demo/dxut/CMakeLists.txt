file(GLOB SRC_LIST *.cpp)
file(GLOB PROJECT_HEADERS include/*.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(DXUT ${SRC_LIST} ${PROJECT_HEADERS})


