SET (EXE_NAME cli)
add_executable(${EXE_NAME}
    "ace-selector.cpp"
    "alternative-selection.cpp"
    "and-condition.cpp"
    "apply-to-range-condition.cpp"
    "condition.cpp"
    "extravagant-conditions.cpp"
    "main.cpp"
    "medici-condition.cpp"
    "move-condition.cpp"
    "one-card-condition.cpp"
    "sequence.cpp"
    "storage.cpp"
)
target_link_libraries (${EXE_NAME}
    source-library
    pthread
)
