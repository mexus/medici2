#include <easylogging++.h>
#include <gtest/gtest.h>

INITIALIZE_EASYLOGGINGPP

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";
    ::testing::FLAGS_gtest_color = "yes";
    return RUN_ALL_TESTS();
}
