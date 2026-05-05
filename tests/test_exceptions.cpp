#include <gtest/gtest.h>
#include "exception.hpp"

using namespace lab2;

TEST(LabException, Constructor) {
    LabException e("тестовое сообщение");
    EXPECT_STREQ(e.what(), "тестовое сообщение");
}

TEST(InvalidArgument, ConstructorAndThrow) {
    InvalidArgument e("ошибка, недоступно");
    EXPECT_STREQ(e.what(), "ошибка, недоступно");
    
    try {
        throw InvalidArgument("ошибка");
        FAIL();
    } catch (const InvalidArgument& e) {
        EXPECT_STREQ(e.what(), "ошибка");
    }
}

TEST(IndexOutOfRange, ConstructorAndThrow) {
    IndexOutOfRange e("ошибка, индекс");
    EXPECT_STREQ(e.what(), "ошибка, индекс");
    
    try {
        throw IndexOutOfRange("ошибка");
        FAIL();
    } catch (const IndexOutOfRange& e) {
        EXPECT_STREQ(e.what(), "ошибка");
    }
}

TEST(EmptyCollection, ConstructorAndThrow) {
    EmptyCollection e("ошибка, пусто");
    EXPECT_STREQ(e.what(), "ошибка");
    
    try {
        throw EmptyCollection("ошибка");
        FAIL();
    } catch (const EmptyCollection& e) {
        EXPECT_STREQ(e.what(), "ошибка");
    }
}

TEST(ExceptionMultiple, CatchAsBase) {
    try {
        throw IndexOutOfRange("вне диапазона");
    } catch (const LabException& e) {
        EXPECT_STREQ(e.what(), "вне диапазона");
    }
}