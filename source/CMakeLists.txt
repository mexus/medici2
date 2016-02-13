add_library (source-library
    "cards/and-condition.cpp"
    "cards/apply-to-range-condition.cpp"
    "cards/card.cpp"
    "cards/move-condition.cpp"
    "cards/one-card-condition.cpp"
    "cards/sequence.cpp"
    "cards/storage.cpp"

    "helpers/sources/time_measure.cpp"

    "i-ching/i-ching.cpp"
    "i-ching/patience-selector.cpp"

    "medici/patience-selector.cpp"
    "medici/patience-target-card-selector.cpp"
    "medici/patience.cpp"

    "mixer/sources/factory.cpp"
)

include_directories(${GTEST_INCLUDE_DIRS})
add_executable("source-test"
        "helpers/tests/time_measure.cpp"
        "mixer/tests/factory.cpp"
        "mixer/tests/mixer.cpp"
)
target_link_libraries("source-test" "source-library" ${GTEST_BOTH_LIBRARIES})
add_test(source "source-test")
