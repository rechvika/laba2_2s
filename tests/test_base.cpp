#include <gtest/gtest.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    
    testing::GTEST_FLAG(print_time) = true;
    testing::GTEST_FLAG(break_on_failure) = false;
    
    return RUN_ALL_TESTS();
}