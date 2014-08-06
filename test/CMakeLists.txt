SET (TEST_EXE medici-test)

add_executable(${TEST_EXE}
        main.cpp
        test_fw.cpp

	modules/test_mixer.cpp
)
target_link_libraries (${TEST_EXE}
	source-library
        pthread
)
