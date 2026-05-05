#include <gtest/gtest.h>
#include "bit.hpp"

using namespace lab2;

TEST(BitConstructor, Default) {
    Bit b;
    EXPECT_EQ(b.Value(), 0);
    EXPECT_EQ(b.ToChar(), '0');
}

TEST(BitConstructor, Bool) {
    Bit b1(true), b2(false);
    EXPECT_EQ(b1.Value(), 1);
    EXPECT_EQ(b2.Value(), 0);
}

TEST(BitConstructor, Int) {
    Bit b1(5), b2(0), b3(-3);
    EXPECT_EQ(b1.Value(), 1);
    EXPECT_EQ(b2.Value(), 0);
    EXPECT_EQ(b3.Value(), 1);
}

TEST(BitOperators, And) {
    Bit b1(true), b2(true), b3(false);
    EXPECT_EQ((b1 & b2).Value(), 1);
    EXPECT_EQ((b1 & b3).Value(), 0);
}

TEST(BitOperators, Or) {
    Bit b1(true), b2(false);
    EXPECT_EQ((b1 | b2).Value(), 1);
    EXPECT_EQ((b2 | b2).Value(), 0);
}

TEST(BitOperators, Xor) {
    Bit b1(true), b2(true), b3(false);
    EXPECT_EQ((b1 ^ b2).Value(), 0);
    EXPECT_EQ((b1 ^ b3).Value(), 1);
}

TEST(BitOperators, Not) {
    Bit b1(true), b2(false);
    EXPECT_EQ((~b1).Value(), 0);
    EXPECT_EQ((~b2).Value(), 1);
}

TEST(BitOperators, Equality) {
    Bit b1(true), b2(true), b3(false);
    EXPECT_EQ(b1 == b2, 1);
    EXPECT_EQ(b1 == b3, 0);
    EXPECT_EQ(b1 != b3, 1);
}

TEST(BitConversion, ToBool) {
    Bit b1(true), b2(false);
    EXPECT_TRUE(static_cast<bool>(b1));
    EXPECT_FALSE(static_cast<bool>(b2));
}

TEST(BitEdgeCases, LargeInt) {
    Bit b(1000000);
    EXPECT_EQ(b.Value(), 1);
    Bit b_neg(-100);
    EXPECT_EQ(b_neg.Value(), 1);
}
