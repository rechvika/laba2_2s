#include <gtest/gtest.h>
#include "array_sequence.hpp"
#include "list_sequence.hpp"
#include "bit_sequence.hpp"
#include "map.hpp"
#include "builder.hpp"

using namespace lab2;

TEST(size_tegration, BuilderToMapToWhere) {
    Builder<size_t> builder;
    builder.Add(1).Add(2).Add(3).Add(4).Add(5);
    
    MutableArraySequence<size_t>* seq = builder.BuildMutableArray();
    Sequence<size_t>* squared = Map(*seq, std::function<size_t(const size_t&)>([](const size_t& x) { return x * x; }));
    delete seq;
    
    Sequence<size_t>* evenSquares = squared->Where([](const size_t& x) { return x % 2 == 0; });
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
    size_t array1[] = {1, 2, 3};
    size_t array2[] = {4, 5, 6};
    MutableArraySequence<size_t> left(array1, 3);
    MutableArraySequence<size_t> right(array2, 3);
    
    Sequence<std::pair<size_t, size_t>>* zipped = Zip(left, right);
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
    MutableArraySequence<size_t> numbers;
    for (size_t i = 1; i <= 10; ++i) numbers.Append(i);
    
    Sequence<size_t>* evens = numbers.Where([](const size_t& x) { return x % 2 == 0; });
    Sequence<size_t>* squares = Map(*evens, std::function<size_t(const size_t&)>([](const size_t& x) { return x * x; }));
    delete evens;
    
    size_t sum = squares->Reduce<size_t>(0, std::function<size_t(const size_t&, const size_t&)>([](const size_t& acc, const size_t& x) { return acc + x; }));
    delete squares;
    
    EXPECT_EQ(sum, 220);
}

TEST(Integration, DifferentStorageTypes) {
    size_t data[] = {1, 2, 3, 4, 5};
    
    MutableArraySequence<size_t> arraySeq(data, 5);
    MutableListSequence<size_t> listSeq(data, 5);
    ImmutableArraySequence<size_t> immutableArray(data, 5);
    
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
    
  Sequence<int>* doubled = Map(*range, std::function<int(const int&)>([](const int& x) { return x * 2; }));
    delete range;
    
    EXPECT_EQ(doubled->Get(0), 0);
    EXPECT_EQ(doubled->Get(4), 16);
    delete doubled;
}