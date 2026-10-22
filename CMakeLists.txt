cmake_minimum_required(VERSION 3.20)
project(learning CXX)

# Use C++20, no compiler-specific extras
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build with debug information so the debugger works
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# Turn on warnings for every program
add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)

##===========
# Programs
##===========
add_executable(ch0_hello ch0/hello.cpp)
add_executable(scratch scratch.cpp)
add_executable(ch01L5Q2 ch01/Lesson5_Quiz2.cpp)
add_executable(ch01LXQ3 ch01/LessonX_Quiz3.cpp)
add_executable(ch02L4Q5 ch02/Lesson4_Quiz5.cpp)
# Multiple files in one program
add_executable(ch02L8Q1 
ch02/L8Q1/main.cpp 
ch02/L8Q1/input.cpp
)
add_executable(ch02LxQ1_3
ch02/LxQ1_3/main.cpp
ch02/LxQ1_3/io.cpp
)
add_executable(ch03LxQ1
ch03/LxQ1.cpp
)
add_executable(ch03LxQ2
ch03/LxQ2.cpp
)
add_executable(ch03LxQ4
ch03/LxQ4.cpp
)