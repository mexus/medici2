cmake_minimum_required (VERSION 2.6)
project(medici)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wall -Wextra -O3")

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include_directories("source")

add_subdirectory(converter)
add_subdirectory(source)
add_subdirectory(test)
add_subdirectory(performance)
add_subdirectory(gui)
