#include <gtest/gtest.h>
#include "bit_sequence.hpp"

using namespace lab2;

TEST(BitSequenceConstructor, Default) {
    BitSequence seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(BitSequenceConstructor, FromString) {
    BitSequence seq("10110");
    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(0).ToChar(), '1');
    EXPECT_EQ(seq.Get(4).ToChar(), '0');
}

TEST(BitSequenceConstructor, FromStringInvalid) {
    EXPECT_THROW(BitSequence seq("10201"), InvalidArgument);
    EXPECT_THROW(BitSequence seq("abc"), InvalidArgument);
}

TEST(BitSequenceConstructor, FromBitArray) {
    Bit bits[] = {Bit(1), Bit(0), Bit(1)};
    BitSequence seq(bits, 3);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(BitSequenceConstructor, Copy) {
    BitSequence original("101");
    BitSequence copy(original);
    EXPECT_EQ(copy.ToBitString(), "101");
}

TEST(BitSequenceAssignment, Operator) {
    BitSequence seq1("101"), seq2("010");
    seq1 = seq2;
    EXPECT_EQ(seq1.ToBitString(), "010");
}

TEST(BitSequenceGetFirstLast, Normal) {
    BitSequence seq("10110");
    EXPECT_EQ(seq.GetFirst().ToChar(), '1');
    EXPECT_EQ(seq.GetLast().ToChar(), '0');
}

TEST(BitSequenceGetFirstLast, Empty) {
    BitSequence seq;
    EXPECT_THROW(seq.GetFirst(), EmptyCollection);
    EXPECT_THROW(seq.GetLast(), EmptyCollection);
}

TEST(BitSequenceGet, Valid) {
    BitSequence seq("101");
    EXPECT_EQ(seq.Get(0).ToChar(), '1');
    EXPECT_EQ(seq.Get(2).ToChar(), '1');
}

TEST(BitSequenceGet, Invalid) {
    BitSequence seq("101");
    EXPECT_THROW(seq.Get(3), IndexOutOfRange);
}

TEST(BitSequenceSet, Valid) {
    BitSequence seq("101");
    seq.Set(1, Bit(1));
    EXPECT_EQ(seq.ToBitString(), "111");
}

TEST(BitSequenceAppend, Normal) {
    BitSequence seq;
    seq.Append(Bit(1)); seq.Append(Bit(0)); seq.Append(Bit(1));
    EXPECT_EQ(seq.ToBitString(), "101");
}

TEST(BitSequencePrepend, Normal) {
    BitSequence seq;
    seq.Prepend(Bit(0)); seq.Prepend(Bit(1));
    EXPECT_EQ(seq.ToBitString(), "10");
}

TEST(BitSequenceInsertAt, Normal) {
    BitSequence seq("101");
    seq.InsertAt(Bit(0), 1);
    EXPECT_EQ(seq.ToBitString(), "1001");
}

TEST(BitSequenceInsertAt, Invalid) {
    BitSequence seq("10");
    EXPECT_THROW(seq.InsertAt(Bit(1), 3), IndexOutOfRange);
}

TEST(BitSequenceGetSubsequence, Normal) {
    BitSequence seq("10110");
    Sequence<Bit>* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    delete sub;
}

TEST(BitSequenceSlice, Remove) {
    BitSequence seq("10110");
    seq.Slice(1, 2);
    EXPECT_EQ(seq.ToBitString(), "110");
}

TEST(BitSequenceSlice, Replace) {
    BitSequence seq("10110");
    BitSequence replacement("000");
    seq.Slice(1, 2, &replacement);
    EXPECT_EQ(seq.ToBitString(), "100010");
}

TEST(BitSequenceConcat, Normal) {
    BitSequence seq1("101"), seq2("010");
    seq1.Concat(seq2);
    EXPECT_EQ(seq1.ToBitString(), "101010");
}

TEST(BitSequenceAnd, Normal) {
    BitSequence seq1("1010"), seq2("1100");
    BitSequence* result = seq1.And(seq2);
    EXPECT_EQ(result->ToBitString(), "1000");
    delete result;
}

TEST(BitSequenceAnd, DifferentLengths) {
    BitSequence seq1("101"), seq2("10");
    EXPECT_THROW(seq1.And(seq2), InvalidArgument);
}

TEST(BitSequenceOr, Normal) {
    BitSequence seq1("1010"), seq2("1100");
    BitSequence* result = seq1.Or(seq2);
    EXPECT_EQ(result->ToBitString(), "1110");
    delete result;
}
TEST(BitSequenceXor, Normal) {
    BitSequence seq1("1010"), seq2("1100");
    BitSequence* result =seq1.Xor(seq2);
    EXPECT_EQ(result->ToBitString(), "0110");
    delete result;
}

TEST(BitSequenceNot, Normal) {
    BitSequence seq("1010");
    BitSequence* result = seq.Not();
    EXPECT_EQ(result->ToBitString(), "0101");
    delete result;
}

TEST(BitSequenceToBitString, Normal) {
    BitSequence seq("10110");
    EXPECT_EQ(seq.ToBitString(), "10110");
}

TEST(BitSequenceClone, Normal) {
    BitSequence seq("101");
    Sequence<Bit>* cloned = seq.Clone();
    seq.Set(0, Bit(0));
    EXPECT_EQ(cloned->Get(0).ToChar(), '1');
    delete cloned;
}

TEST(BitSequenceStorageName, Name) {
    BitSequence seq;
    EXPECT_STREQ(seq.StorageName(), "BitSequence");
}

TEST(BitSequenceSlice, NullReplacement) {
    BitSequence seq("10110");
    EXPECT_NO_THROW(seq.Slice(1, 2, nullptr));
    EXPECT_EQ(seq.ToBitString(), "110");
}

TEST(BitSequenceInsertAt, EmptyCollectionPrepend) {
    BitSequence seq;
    EXPECT_NO_THROW(seq.Prepend(Bit(1)));
    EXPECT_EQ(seq.ToBitString(), "1");
}