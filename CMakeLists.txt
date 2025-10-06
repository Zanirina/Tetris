cmake_minimum_required(VERSION 3.10)
project(TetrisPBR)

set(CMAKE_CXX_STANDARD 17)

# Пусть CMake знает, где vcpkg
set(CMAKE_TOOLCHAIN_FILE "C:/Users/22208/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)

file(GLOB SRC_FILES
        src/*.cpp
        src/*.h
)

add_executable(TetrisPBR ${SRC_FILES})

target_link_libraries(TetrisPBR PRIVATE glfw glad::glad glm::glm-header-only)
target_include_directories(TetrisPBR PRIVATE ${Stb_INCLUDE_DIRS})
