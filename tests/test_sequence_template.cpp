#include <gtest/gtest.h>
#include "array_sequence.hpp"

using namespace lab2;

TEST(SequenceTryFirst, Normal) {
    int array[] = {10, 20, 30};
    MutableArraySequence<int> seq(array, 3);
    
    Option<int> result = seq.TryFirst();
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value(), 10);
    
    Option<int> result2 = seq.TryFirst([](const int& x) { return x > 15; });
    EXPECT_TRUE(result2.HasValue());
    EXPECT_EQ(result2.Value(), 20);
}

TEST(SequenceTryFirst, EmptyOrNoMatch) {
    MutableArraySequence<int> seq;
    EXPECT_FALSE(seq.TryFirst().HasValue());
    
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq2(array, 3);
    EXPECT_FALSE(seq2.TryFirst([](const int& x) { return x > 10; }).HasValue());
}

TEST(SequenceTryLast, Normal) {
    int array[] = {10, 20, 30};
    MutableArraySequence<int> seq(array, 3);
    
    Option<int> result = seq.TryLast();
    EXPECT_TRUE(result.HasValue());
    EXPECT_EQ(result.Value(), 30);
    
    Option<int> result2 = seq.TryLast([](const int& x) { return x < 25; });
    EXPECT_TRUE(result2.HasValue());
    EXPECT_EQ(result2.Value(), 20);
}

TEST(SequenceTryLast, Empty) {
    MutableArraySequence<int> seq;
    EXPECT_FALSE(seq.TryLast().HasValue());
}

TEST(SequenceReduce, Normal) {
    int array[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(array, 4);
    
    int sum = seq.Reduce(0, [](const int& acc, const int& x) { return acc + x; });
    EXPECT_EQ(sum, 10);
    
    int product = seq.Reduce(1, [](const int& acc, const int& x) { return acc * x; });
    EXPECT_EQ(product, 24);
}

TEST(SequenceReduce, Empty) {
    MutableArraySequence<int> seq;
    int result = seq.Reduce(42, [](const int& acc, const int& x) { return acc + x; });
    EXPECT_EQ(result, 42);
}

TEST(SequenceWhere, Normal) {
    int array[] = {1, 2, 3, 4, 5, 6};
    MutableArraySequence<int> seq(array, 6);
    
    Sequence<int>* evens = seq.Where([](const int& x) { return x % 2 == 0; });
    EXPECT_EQ(evens->GetLength(), 3);
    EXPECT_EQ(evens->Get(0), 2);
    EXPECT_EQ(evens->Get(2), 6);
    delete evens;
    
    Sequence<int>* empty = seq.Where([](const int& x) { return x > 10; });
    EXPECT_EQ(empty->GetLength(), 0);
    delete empty;
}

TEST(SequenceBracketOperator, Valid) {
    int array[] = {5, 10, 15};
    MutableArraySequence<int> seq(array, 3);
    EXPECT_EQ(seq[0], 5);
    EXPECT_EQ(seq[2], 15);
}

TEST(SequenceBracketOperator, Invalid) {
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq(array, 3);
    EXPECT_THROW(seq[3], IndexOutOfRange);
}

TEST(SequenceEnumerator, Enumerate) {
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq(array, 3);
    auto enumerator = seq.GetEnumerator();
    
    EXPECT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->Current(), 1);
    EXPECT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->Current(), 2);
    EXPECT_TRUE(enumerator->MoveNext());
    EXPECT_EQ(enumerator->Current(), 3);
    EXPECT_FALSE(enumerator->MoveNext());
}

TEST(SequenceEnumerator, Reset) {
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq(array, 3);
    auto enumerator = seq.GetEnumerator();
    
    enumerator->MoveNext();
    EXPECT_EQ(enumerator->Current(), 1);
    enumerator->Reset();
    enumerator->MoveNext();
    EXPECT_EQ(enumerator->Current(), 1);
}