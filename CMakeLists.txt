cmake_minimum_required(VERSION 3.23)
project(CubeRubik)

set(CMAKE_CXX_STANDARD 14)

add_executable(CubeRubik main.cpp CubePiece.h CubePiece.cpp Cube.h Cube.cpp)

target_link_libraries(CubeRubik -lOpenGL32 -lfreeGLUT -lglu32)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})