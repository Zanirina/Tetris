cmake_minimum_required(VERSION 3.10)
project(TetrisPBR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Найти зависимости
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Источники
file(GLOB SRC_FILES
        src/*.cpp
        src/*.h
        src/imgui_impl/*.cpp
)

# Создаем исполняемый файл
add_executable(TetrisPBR ${SRC_FILES})

# Линковка библиотек
target_link_libraries(TetrisPBR PRIVATE
        glfw
        glad::glad
        glm::glm-header-only
        imgui::imgui
)

# Заголовочные-only библиотеки (stb)
target_include_directories(TetrisPBR PRIVATE
        "C:/vcpkg/installed/x64-windows/include"
)
