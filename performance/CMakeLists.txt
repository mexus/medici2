SET (TEST_EXE performance-test)
add_executable(${TEST_EXE}
	main.cpp
	performance.cpp
)
target_link_libraries (${TEST_EXE}
	source-library
        pthread
)
