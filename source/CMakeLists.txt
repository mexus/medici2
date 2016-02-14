add_library (source-library
    "cards/sources/and-condition.cpp"
    "cards/sources/any-rank-condition.cpp"
    "cards/sources/any-suit-condition.cpp"
    "cards/sources/apply-to-range-condition.cpp"
    "cards/sources/card.cpp"
    "cards/sources/move-condition.cpp"
    "cards/sources/one-card-condition.cpp"
    "cards/sources/sequence.cpp"
    "cards/sources/simple-sequence.cpp"
    "cards/sources/storage.cpp"

    "helpers/sources/sequence-mixer.cpp"
    "helpers/sources/time_measure.cpp"

    "i-ching/sources/i-ching.cpp"
    "i-ching/sources/patience-selector.cpp"

    "medici/sources/patience-selector.cpp"
    "medici/sources/patience-target-card-selector.cpp"
    "medici/sources/patience.cpp"

    "mixer/sources/factory.cpp"
)

include_directories(${GTEST_INCLUDE_DIRS})
add_executable("source-test"
    "cards/tests/and-condition.cpp"
    "cards/tests/any-rank-condition.cpp"
    "cards/tests/any-suit-condition.cpp"
    "cards/tests/apply-to-range-condition.cpp"
    "cards/tests/card.cpp"
    "cards/tests/move-condition.cpp"
    "cards/tests/one-card-condition.cpp"
    "cards/tests/sequence.cpp"
    "cards/tests/simple-sequence.cpp"
    "cards/tests/storage.cpp"

    "helpers/tests/sequence-mixer.cpp"
    "helpers/tests/time_measure.cpp"

    "i-ching/tests/i-ching.cpp"
    "i-ching/tests/patience-selector.cpp"

    "medici/tests/patience-selector.cpp"
    "medici/tests/patience.cpp"

    "mixer/tests/factory.cpp"
    "mixer/tests/mixer.cpp"
)
target_link_libraries("source-test" "source-library" "gtest-main")
add_test(source "source-test")
