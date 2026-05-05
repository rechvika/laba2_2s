#include <gtest/gtest.h>
#include "array_sequence.hpp"

using namespace lab2;

class ArraySequenceTestHelper {
public:
    template<class T>
    static void CallValidateClosedRange(const ArraySequence<T>* seq, size_t start, size_t end) {
        seq->ValidateClosedRange(start, end);
    }
    template<class T>
    static size_t CallNormalizeSliceIndex(const ArraySequence<T>* seq, size_t index) {
        return seq->NormalizeSliceIndex(index);
    }
    template<class T>
    static ArraySequence<T>* CallAppendInternal(ArraySequence<T>* seq, const T& item) {
        return seq->AppendInternal(item);
    }
    template<class T>
    static ArraySequence<T>* CallPrepareForWrite(ArraySequence<T>* seq) {
        return seq->PrepareForWrite();
    }
};

TEST(MutableArraySequenceConstructor, Default) {
    MutableArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(MutableArraySequenceConstructor, FromArray) {
    int array[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(array, 5);
    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(4), 5);
}

TEST(MutableArraySequenceConstructor, FromList) {
    LinkedList<int> list;
    list.Append(10); list.Append(20); list.Append(30);
    MutableArraySequence<int> seq(list);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 10);
}

TEST(MutableArraySequenceConstructor, Copy) {
    int array[] = {5, 6, 7, 8};
    MutableArraySequence<int> original(array, 4);
    MutableArraySequence<int> copy(original);
    original.Set(0, 100);
    EXPECT_EQ(copy.Get(0), 5);
}

TEST(MutableArraySequenceGetFirstLast, Normal) {
    int array[] = {100, 200, 300};
    MutableArraySequence<int> seq(array, 3);
    EXPECT_EQ(seq.GetFirst(), 100);
    EXPECT_EQ(seq.GetLast(), 300);
}

TEST(MutableArraySequenceGetFirstLast, Empty) {
    MutableArraySequence<int> seq;
    EXPECT_THROW(seq.GetFirst(), EmptyCollection);
    EXPECT_THROW(seq.GetLast(), EmptyCollection);
}

TEST(MutableArraySequenceGet, Valid) {
    int array[] = {10, 20, 30, 40};
    MutableArraySequence<int> seq(array, 4);
    EXPECT_EQ(seq.Get(0), 10);
    EXPECT_EQ(seq.Get(3), 40);
}

TEST(MutableArraySequenceGet, Invalid) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    EXPECT_THROW(seq.Get(-1), IndexOutOfRange);
    EXPECT_THROW(seq.Get(1), IndexOutOfRange);
}

TEST(MutableArraySequenceSet, Valid) {
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq(array, 3);
    seq.Set(1, 99);
    EXPECT_EQ(seq.Get(1), 99);
}

TEST(MutableArraySequenceSet, Invalid) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    EXPECT_THROW(seq.Set(1, 10), IndexOutOfRange);
}

TEST(MutableArraySequenceAppend, Normal) {
    MutableArraySequence<int> seq;
    seq.Append(1); seq.Append(2); seq.Append(3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(MutableArraySequencePrepend, Normal) {
    MutableArraySequence<int> seq;
    seq.Prepend(3); seq.Prepend(2); seq.Prepend(1);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(MutableArraySequenceInsertAt, Normal) {
    MutableArraySequence<int> seq;
    seq.Append(1); seq.Append(3); seq.Append(4);
    seq.InsertAt(2, 1);
    EXPECT_EQ(seq.Get(1), 2);
}

TEST(MutableArraySequenceInsertAt, Invalid) {
    MutableArraySequence<int> seq;
    seq.Append(1);
    EXPECT_THROW(seq.InsertAt(5, 2), IndexOutOfRange);
}

TEST(MutableArraySequenceGetSubsequence, Normal) {
    int array[] = {10, 20, 30, 40, 50};
    MutableArraySequence<int> seq(array, 5);
    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 20);
    delete sub;
}

TEST(MutableArraySequenceGetSubsequence, Invalid) {
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq(array, 3);
    EXPECT_THROW(seq.GetSubsequence(2, 1), InvalidArgument);
    EXPECT_THROW(seq.GetSubsequence(0, 5), IndexOutOfRange);
}

TEST(MutableArraySequenceSlice,Remove) {
    int array[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(array, 5);
    seq.Slice(1, 2);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(1), 4);
}

TEST(MutableArraySequenceSlice, Replace) {
    int array[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(array, 5);
    int replacement_items[] = {10, 20};
    MutableArraySequence<int> replacement(replacement_items, 2);
    seq.Slice(1, 2, &replacement);
    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(1), 10);
}

TEST(MutableArraySequenceSlice, Invalid) {
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq(array, 3);
    EXPECT_THROW(seq.Slice(0, -1), InvalidArgument);
}

TEST(MutableArraySequenceConcat, Normal) {
    int array1[] = {1, 2, 3};
    int array2[] = {4, 5, 6};
    MutableArraySequence<int> seq1(array1, 3);
    MutableArraySequence<int> seq2(array2, 3);
    seq1.Concat(seq2);
    EXPECT_EQ(seq1.GetLength(), 6);
}

TEST(MutableArraySequenceClone, Normal) {
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq(array, 3);
    Sequence<int>* cloned = seq.Clone();
    seq.Set(0, 100);
    EXPECT_EQ(cloned->Get(0), 1);
    delete cloned;
}

TEST(MutableArraySequenceStorageName, Name) {
    MutableArraySequence<int> seq;
    EXPECT_STREQ(seq.StorageName(), "MutableArraySequence");
}
TEST(ImmutableArraySequenceConstructor, Default) {
    ImmutableArraySequence<int> seq;
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(ImmutableArraySequenceConstructor, FromArray) {
    int array[] = {10, 20, 30, 40};
    ImmutableArraySequence<int> seq(array, 4);
    EXPECT_EQ(seq.GetLength(), 4);
}

TEST(ImmutableArraySequenceModification, ReturnsNew) {
    ImmutableArraySequence<int> seq;
    Sequence<int>* new_seq = seq.Append(5);
    EXPECT_NE(new_seq, &seq);
    EXPECT_EQ(new_seq->GetLength(), 1);
    delete new_seq;
}

TEST(ImmutableArraySequenceStorageName, Name) {
    ImmutableArraySequence<int> seq;
    EXPECT_STREQ(seq.StorageName(), "ImmutableArraySequence");
}

TEST(ArraySequencePrivate, ValidateClosedRange) {
    int array[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(array, 5);
    EXPECT_NO_THROW(ArraySequenceTestHelper::CallValidateClosedRange(&seq, 1, 3));
    EXPECT_THROW(ArraySequenceTestHelper::CallValidateClosedRange(&seq, 3, 1), InvalidArgument);
}

TEST(ArraySequencePrivate, NormalizeSliceIndex) {
    int array[] = {1, 2, 3};
    MutableArraySequence<int> seq(array, 3);
    EXPECT_EQ(ArraySequenceTestHelper::CallNormalizeSliceIndex(&seq, 2), 2);
    EXPECT_THROW(ArraySequenceTestHelper::CallNormalizeSliceIndex(&seq, 10), IndexOutOfRange);
}

TEST(ArraySequencePrivate, AppendInternal) {
    MutableArraySequence<int> seq;
    ArraySequenceTestHelper::CallAppendInternal(&seq, 42);
    EXPECT_EQ(seq.GetLength(), 1);
}

TEST(ArraySequencePrivate, PrepareForWrite) {
    MutableArraySequence<int> mutable_seq;
    ImmutableArraySequence<int> immutable_seq;
    EXPECT_EQ(ArraySequenceTestHelper::CallPrepareForWrite(&mutable_seq), &mutable_seq);
    EXPECT_NE(ArraySequenceTestHelper::CallPrepareForWrite(&immutable_seq), &immutable_seq);
}