#include <gtest/gtest.h>
#include "list_sequence.hpp"

using namespace lab2;

TEST(MutableListSequenceGetSubsequence, EmptyCollection) {
    MutableListSequence<int> seq;
    EXPECT_THROW(seq.GetSubsequence(0, 0), EmptyCollection);
}

TEST(MutableListSequenceSlice, NullReplacement) {
    int array[] = {1, 2, 3, 4, 5};
    MutableListSequence<int> seq(array, 5);
    EXPECT_NO_THROW(seq.Slice(1, 2, nullptr));
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableListSequenceInsertAt, InvalidIndexPastLength) {
    MutableListSequence<int> seq;
    seq.Append(1);
    EXPECT_THROW(seq.InsertAt(2, 2), IndexOutOfRange);
}

TEST(ImmutableListSequenceModification, ReturnsNew) {
    ImmutableListSequence<int> seq;
    Sequence<int>* new_seq = seq.Append(5);
    EXPECT_NE(new_seq, &seq);
    EXPECT_EQ(new_seq->GetLength(), 1);
    delete new_seq;
}