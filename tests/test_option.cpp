#include <gtest/gtest.h>
#include "option.hpp"

using namespace lab2;

TEST(OptionConstructor, Default) {
    Option<int> opt;
    EXPECT_FALSE(opt.HasValue());
    EXPECT_FALSE(static_cast<bool>(opt));
}

TEST(OptionConstructor, Value) {
    Option<int> opt(42);
    EXPECT_TRUE(opt.HasValue());
    EXPECT_EQ(opt.Value(), 42);
}

TEST(OptionConstructor, Copy) {
    Option<int> original(100);
    Option<int> copy(original);
    EXPECT_TRUE(copy.HasValue());
    EXPECT_EQ(copy.Value(), 100);
}

TEST(OptionHasValue, Check) {
    Option<int> opt1(10), opt2;
    EXPECT_TRUE(opt1.HasValue());
    EXPECT_FALSE(opt2.HasValue());
}

TEST(OptionBoolConversion, Check) {
    Option<int> opt1(42), opt2;
    EXPECT_TRUE(static_cast<bool>(opt1));
    EXPECT_FALSE(static_cast<bool>(opt2));
}

TEST(OptionValue, Access) {
    Option<int> opt(123);
    EXPECT_EQ(opt.Value(), 123);
    opt.Value() = 456;
    EXPECT_EQ(opt.Value(), 456);
}

TEST(OptionValue, EmptyThrows) {
    Option<int> opt;
    EXPECT_THROW(opt.Value(), std::runtime_error);
}

TEST(OptionValueOr, Normal) {
    Option<int> opt1(42), opt2;
    EXPECT_EQ(opt1.ValueOr(0), 42);
    EXPECT_EQ(opt2.ValueOr(99), 99);
}

TEST(OptionNone, ReturnsEmpty) {
    Option<int> opt = Option<int>::None();
    EXPECT_FALSE(opt.HasValue());
}

TEST(OptionType, DifferentTypes) {
    Option<std::string> opt1("hello");
    EXPECT_EQ(opt1.Value(), "hello");
    
    Option<double> opt2(3.14);
    EXPECT_DOUBLE_EQ(opt2.Value(), 3.14);
    
    Option<bool> opt3(true);
    EXPECT_EQ(opt3.Value(), true);
}