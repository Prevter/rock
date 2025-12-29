cmake_minimum_required(VERSION 3.21)
project(rock)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ROCK_BUILD_DEMO "Build the demo project" OFF)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
target_sources(${PROJECT_NAME} INTERFACE
    src/RoundedRect.cpp
    src/Utils.cpp
)

if (ROCK_BUILD_DEMO)
    add_subdirectory(demo)
endif()