#include <gtest/gtest.h>
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "bit_sequence.hpp"
#include "map.hpp"
#include "builder.hpp"

using namespace lab2;

TEST(Integration, BuilderToMapToWhere) {
    Builder<int> builder;
    builder.Add(1).Add(2).Add(3).Add(4).Add(5);
    
    MutableArraySequence<int>* seq = builder.BuildMutableArray();
    Sequence<int>* squared = Map(*seq, [](const int& x) { return x * x; });
    delete seq;
    
    Sequence<int>* evenSquares = squared->Where([](const int& x) { return x % 2 == 0; });
    delete squared;
    
    EXPECT_EQ(evenSquares->GetLength(), 2);
    EXPECT_EQ(evenSquares->Get(0), 4);
    EXPECT_EQ(evenSquares->Get(1), 16);
    delete evenSquares;
}

TEST(Integration, BitSequenceChainOperations) {
    BitSequence seq1("1010"), seq2("1100");
    
    BitSequence* andResult = seq1.And(seq2);
    BitSequence* orResult = seq1.Or(seq2);
    BitSequence* xorResult = seq1.Xor(seq2);
    
    EXPECT_EQ(andResult->ToBitString(), "1000");
    EXPECT_EQ(orResult->ToBitString(), "1110");
    EXPECT_EQ(xorResult->ToBitString(), "0110");
    
    delete andResult;
    delete orResult;
    delete xorResult;
}

TEST(Integration, ZipUnzipRoundTrip) {
    int array1[] = {1, 2, 3};
    int array2[] = {4, 5, 6};
    MutableArraySequence<int> left(array1, 3);
    MutableArraySequence<int> right(array2, 3);
    
    Sequence<std::pair<int, int>>* zipped = Zip(left, right);
    auto unzipped = Unzip(*zipped);
    delete zipped;
    
    for (size_t i = 0; i < 3; ++i) {
        EXPECT_EQ(unzipped.first->Get(i), left.Get(i));
        EXPECT_EQ(unzipped.second->Get(i), right.Get(i));
    }
    
    delete unzipped.first;
    delete unzipped.second;
}

TEST(Integration, ComplexPipeline) {
    MutableArraySequence<int> numbers;
    for (int i = 1; i <= 10; ++i) numbers.Append(i);
    
    Sequence<int>* evens = numbers.Where([](const int& x) { return x % 2 == 0; });
    Sequence<int>* squares = Map(*evens, [](const int& x) { return x * x; });
    delete evens;
    
    int sum = squares->Reduce(0, [](const int& acc, const int& x) { return acc + x; });
    delete squares;
    
    EXPECT_EQ(sum, 220);
}

TEST(Integration, DifferentStorageTypes) {
    int data[] = {1, 2, 3, 4, 5};
    
    MutableArraySequence<int> arraySeq(data, 5);
    MutableListSequence<int> listSeq(data, 5);
    ImmutableArraySequence<int> immutableArray(data, 5);
    
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arraySeq.Get(i), listSeq.Get(i));
        EXPECT_EQ(immutableArray.Get(i), listSeq.Get(i));
    }
    
    arraySeq.Set(0, 100);
    EXPECT_EQ(immutableArray.Get(0), 1);
}

TEST(Integration, RangeOperations) {
    Sequence<int>* range = Range(0, 10, 2);
    EXPECT_EQ(range->GetLength(), 5);
    
    Sequence<int>* doubled = Map(*range, [](const int& x) { return x * 2; });
    delete range;
    
    EXPECT_EQ(doubled->Get(0), 0);
    EXPECT_EQ(doubled->Get(4), 16);
    delete doubled;
}