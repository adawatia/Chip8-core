cmake_minimum_required(VERSION 3.20)

project(CHIP8-Emulator
    VERSION 1.0
    DESCRIPTION "A CHIP-8 emulator built in modern C++"
    LANGUAGES CXX
)

# Set C++23 as the standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable all reasonable warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Add third-party libraries ==> Pending
add_subdirectory(3rdParty/glad EXCLUDE_FROM_ALL)
add_subdirectory(3rdParty/sdl-2.0.20 EXCLUDE_FROM_ALL)
add_subdirectory(3rdParty/imgui-1.88 EXCLUDE_FROM_ALL)

# Collect all source files
set(SOURCES
    Source/Chip8.cpp
    Source/Main.cpp
    Source/Platform.cpp
)

# Define the main executable target
add_executable(chip8 ${SOURCES})

# Set include paths for internal and third-party headers
target_include_directories(chip8 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    3rdParty/glad/include
    3rdParty/sdl-2.0.20/include
    3rdParty/imgui-1.88
)

# Link the libraries needed
target_link_libraries(chip8 PRIVATE
    glad
    SDL2
    imgui
)
