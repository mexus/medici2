cmake_minimum_required (VERSION 2.8.5)
project(medici)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wall -Wextra -O3 -g")
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories("source")

find_package(GTest REQUIRED)
enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} "--output-on-failure")

add_subdirectory(gtest-main)
add_subdirectory(converter)
add_subdirectory(source)
# add_subdirectory(performance)
add_subdirectory(cli)
