SET (EXE_NAME cli)
add_executable(${EXE_NAME}
    ace-selector.cpp
    cli.cpp
    extravagant-conditions.cpp
    main.cpp
)
target_link_libraries (${EXE_NAME}
    source-library
    pthread
)
