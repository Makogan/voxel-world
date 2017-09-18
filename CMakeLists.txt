cmake_minimum_required(VERSION 3.9)

project(VWorld)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/middleware")
include_directories("${PROJECT_SOURCE_DIR}/middleware/stb")
include_directories("${PROJECT_SOURCE_DIR}/rendering")
include_directories("${PROJECT_SOURCE_DIR}/rendering/Camera")
include_directories("${PROJECT_SOURCE_DIR}/Helpers")

include_directories("/usr/lib/nvidia-375")
include_directories("/usr/local/lib")
include_directories("/usr/include/GL")
include_directories("/usr/include/freetype2")
include_directories("/usr/local/lib")

SET(CMAKE_CXX_FLAGS "-std=c++1y -lglfw -lGL -lOpenGL -lGLEW -pthread -lfreetype")

add_executable(voxel-world.exe ${PROJECT_SOURCE_DIR}/main.cpp)
target_sources(voxel-world.exe PUBLIC ${PROJECT_SOURCE_DIR}/rendering/context-creation.cpp)
target_sources(voxel-world.exe PUBLIC ${PROJECT_SOURCE_DIR}/rendering/rendering.cpp)
target_sources(voxel-world.exe PUBLIC ${PROJECT_SOURCE_DIR}/rendering/Camera/Camera.cpp)
target_sources(voxel-world.exe PUBLIC ${PROJECT_SOURCE_DIR}/Helpers/wavefront-loader.cpp)
