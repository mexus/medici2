cmake_minimum_required (VERSION 2.8.5)
project(medici)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_EXTRA_FLAGS "-Wall -Wextra -Werror")

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        SET(CMAKE_CXX_EXTRA_FLAGS "${CMAKE_CXX_EXTRA_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    endif()
endif()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_EXTRA_FLAGS}")

include_directories("source")

find_package(GTest REQUIRED)
enable_testing()
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} "--output-on-failure")

add_subdirectory(gtest-main)
add_subdirectory(converter)
add_subdirectory(source)
add_subdirectory(performance)
add_subdirectory(cli)
