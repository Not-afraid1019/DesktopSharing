cmake_minimum_required(VERSION 3.28)
project(DesktopSharing)

set(CMAKE_CXX_STANDARD 11)

link_directories(libs/glfw/lib-vc2010-32 libs/SDL2/lib/x86 libs/ffmpeg/lib)

include_directories(libs/ffmpeg/include)

include_directories(libs/gl3w)

include_directories(libs/glfw/include)

include_directories(libs/SDL2/include)

include_directories(capture codec imgui libyuv/include codec/QsvCodec/include ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE SRC_LIST ./*.cpp ./*.cc)

add_executable(DesktopSharing main.cpp ${SRC_LIST})

target_link_libraries(DesktopSharing glfw3.lib opengl32.lib SDL2.lib avdevice.lib avformat.lib swresample.lib avcodec.lib swscale.lib avutil.lib dxgi.lib d3d11.lib d3d9.lib dxva2.lib)
