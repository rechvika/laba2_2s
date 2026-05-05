#include <gtest/gtest.h>
#include "map.hpp"
#include "array_sequence.hpp"

using namespace lab2;

TEST(MapFunction, Normal) {
    int array[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(array, 4);
    
    Sequence<int>* squared = Map(seq, [](const int& x) { return x * x; });
    EXPECT_EQ(squared->GetLength(), 4);
    EXPECT_EQ(squared->Get(0), 1);
    EXPECT_EQ(squared->Get(3), 16);
    delete squared;
    
    Sequence<std::string>* strings = Map(seq, [](const int& x) { return std::to_string(x); });
    EXPECT_EQ(strings->Get(0), "1");
    delete strings;
}

TEST(MapFunction, Empty) {
    MutableArraySequence<int> seq;
    Sequence<int>* mapped = Map(seq, [](const int& x) { return x * 2; });
    EXPECT_EQ(mapped->GetLength(), 0);
    delete mapped;
}

TEST(MapIndexedFunction, Normal) {
    int array[] = {10, 20, 30};
    MutableArraySequence<int> seq(array, 3);
    
    Sequence<int>* result = MapIndexed(seq, [](const int& x, size_t i) { return x + i; });
    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(1), 21);
    EXPECT_EQ(result->Get(2), 32);
    delete result;
}

TEST(FlatMapFunction, Normal) {
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq(array, 3);
    
    Sequence<int>* expanded = FlatMap(seq, [](const int& x) {
        DynamicArray<int> result(2);
        result.Set(0, x);
        result.Set(1, x * 10);
        return result;
    });
    EXPECT_EQ(expanded->GetLength(), 6);
    EXPECT_EQ(expanded->Get(0), 1);
    EXPECT_EQ(expanded->Get(5), 30);
    delete expanded;
}

TEST(ZipFunction, Normal) {
    int array1[] = {1, 2, 3};
    int array2[] = {4, 5, 6};
    MutableArraySequence<int> left(array1, 3);
    MutableArraySequence<int> right(array2, 3);
    
    Sequence<std::pair<int, int>>* zipped = Zip(left, right);
    EXPECT_EQ(zipped->GetLength(), 3);
    EXPECT_EQ(zipped->Get(0).first, 1);
    EXPECT_EQ(zipped->Get(0).second, 4);
    delete zipped;
}

TEST(ZipFunction, DifferentLengths) {
    int array1[] = {1, 2, 3, 4};
    int array2[] = {5, 6};
    MutableArraySequence<int> left(array1, 4);
    MutableArraySequence<int> right(array2, 2);
    
    Sequence<std::pair<int, int>>* zipped = Zip(left, right);
    EXPECT_EQ(zipped->GetLength(), 2);
    delete zipped;
}

TEST(UnzipFunction, Normal) {
    MutableArraySequence<std::pair<int, int>> seq;
    seq.Append(std::make_pair(1, 10));
    seq.Append(std::make_pair(2, 20));
    seq.Append(std::make_pair(3, 30));
    
    auto result = Unzip(seq);
    EXPECT_EQ(result.first->GetLength(), 3);
    EXPECT_EQ(result.first->Get(0), 1);
    EXPECT_EQ(result.second->Get(0), 10);
    delete result.first;
    delete result.second;
}

TEST(RangeFunction, PositiveStep) {
    Sequence<int>* result = Range(1, 5, 1);
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(3), 4);
    delete result;
}

TEST(RangeFunction, NegativeStep) {
    Sequence<int>* result = Range(5, 1, -1);
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 5);
    EXPECT_EQ(result->Get(3), 2);
    delete result;
}

TEST(RangeFunction, StepZero) {
    EXPECT_THROW(Range(1, 5, 0), InvalidArgument);
}

TEST(RangeFunction, Empty) {
    Sequence<int>* result = Range(5, 1, 1);
    EXPECT_EQ(result->GetLength(), 0);
    delete result;
}