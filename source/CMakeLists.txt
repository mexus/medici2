#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

add_library (source-library
	logxx/logxx.cpp
	helpers/time_measure.cpp
)

