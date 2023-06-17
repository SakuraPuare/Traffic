cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME Traffic)
project(${PROJECT_NAME})

set(GOOGLETEST_VERSION 1.13.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

include_directories(include)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_directories(${PROJECT_NAME} PUBLIC)