SET (EXE_NAME cli)
add_executable(${EXE_NAME}
    "alternative-selection.cpp"
    "extravagant-patience.cpp"
    "main.cpp"
    "medici-condition.cpp"
)
target_link_libraries (${EXE_NAME}
    source-library
    pthread
)
