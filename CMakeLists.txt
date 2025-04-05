cmake_minimum_required(VERSION 3.10)
project(matchmaking_demo)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
add_executable(matchmaking_demo
    src/main.cpp
)

# Include directories
target_include_directories(matchmaking_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Set compiler flags
if(MSVC)
    target_compile_options(matchmaking_demo PRIVATE /W4)
else()
    target_compile_options(matchmaking_demo PRIVATE -Wall -Wextra)
endif()