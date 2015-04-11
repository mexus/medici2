set (TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
configure_file (
    "config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(".")

SET (TEST_EXE medici-test)
add_executable(${TEST_EXE}
    main.cpp
    test_fw.cpp

    modules/operators.cpp

    modules/mixer.cpp
    modules/medici.cpp
    modules/deck-selector.cpp
    modules/i-ching.cpp
    modules/calculator-thread.cpp
    modules/calculator-manager.cpp
)
target_link_libraries (${TEST_EXE}
    source-library
    pthread
)
