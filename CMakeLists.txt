cmake_minimum_required (VERSION 2.8.5)
project(medici)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wall -Wextra -O3 -g")
SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories("source")

add_subdirectory(converter)
add_subdirectory(source)
# add_subdirectory(performance)
add_subdirectory(cli)
