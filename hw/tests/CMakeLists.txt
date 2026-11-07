cmake_minimum_required(VERSION 3.20)
project(hw_tests CXX)

include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
FetchContent_MakeAvailable(googletest)

add_library(cli_parser ../arduino/c3_cli/cli_parser.cpp)
target_include_directories(cli_parser PUBLIC ../arduino/c3_cli)
set_target_properties(cli_parser PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)
target_compile_options(cli_parser PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_executable(cli_parser_test cli_parser_test.cpp)
set_target_properties(cli_parser_test PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(cli_parser_test cli_parser GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(cli_parser_test)