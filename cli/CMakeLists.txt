SET (EXE_NAME cli)
add_executable(${EXE_NAME}
    main.cpp
    cli.cpp
    ace-selector.cpp
)
target_link_libraries (${EXE_NAME}
    source-library
    pthread
)
