cmake_minimum_required(VERSION 3.5)

project(VWorld)

set(CMAKE_BINARY_DIR .)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)

add_executable(voxel-world ${PROJECT_SOURCE_DIR}/main.cpp)

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/middleware")
include_directories("${PROJECT_SOURCE_DIR}/middleware/stb")
include_directories("${PROJECT_SOURCE_DIR}/Rendering")
include_directories("${PROJECT_SOURCE_DIR}/Rendering/Camera")
include_directories("${PROJECT_SOURCE_DIR}/Helpers")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})

target_link_libraries(voxel-world glfw GL GLEW freetype)

SET(CMAKE_CXX_FLAGS "-std=c++1y")

target_sources(voxel-world PUBLIC ${PROJECT_SOURCE_DIR}/Rendering/context-creation.cpp)
target_sources(voxel-world PUBLIC ${PROJECT_SOURCE_DIR}/Rendering/rendering.cpp)
target_sources(voxel-world PUBLIC ${PROJECT_SOURCE_DIR}/Rendering/Camera/Camera.cpp)
target_sources(voxel-world PUBLIC ${PROJECT_SOURCE_DIR}/Helpers/wavefront-loader.cpp)
target_sources(voxel-world PUBLIC ${PROJECT_SOURCE_DIR}/Helpers/cout-definitions.cpp)
target_sources(voxel-world PUBLIC ${PROJECT_SOURCE_DIR}/Cube.cpp)

file(COPY ${PROJECT_SOURCE_DIR}/Rendering/Shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/Assets DESTINATION ${CMAKE_BINARY_DIR})