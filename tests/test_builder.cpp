#include <gtest/gtest.h>
#include "builder.hpp"

using namespace lab2;

TEST(Builder, AddAndBuildMutableArray) {
    Builder<int> builder;
    builder.Add(1).Add(2).Add(3);
    
    MutableArraySequence<int>* seq = builder.BuildMutableArray();
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(seq->Get(0), 1);
    EXPECT_EQ(seq->Get(2), 3);
    delete seq;
}

TEST(Builder, AddRange) {
    LinkedList<int> list;
    list.Append(5); list.Append(6); list.Append(7);
    
    Builder<int> builder;
    builder.Add(1).Add(2).AddRange(list);
    
    MutableArraySequence<int>* seq = builder.BuildMutableArray();
    EXPECT_EQ(seq->GetLength(), 5);
    EXPECT_EQ(seq->Get(0), 1);
    EXPECT_EQ(seq->Get(4), 7);
    delete seq;
}

TEST(Builder, BuildImmutableArray) {
    Builder<int> builder;
    builder.Add(100).Add(200);
    
    ImmutableArraySequence<int>* seq = builder.BuildImmutableArray();
    EXPECT_EQ(seq->GetLength(), 2);
    EXPECT_EQ(seq->Get(0), 100);
    delete seq;
}

TEST(Builder, BuildMutableList) {
    Builder<int> builder;
    builder.Add(5).Add(6).Add(7);
    
    MutableListSequence<int>* seq = builder.BuildMutableList();
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(seq->Get(0), 5);
    delete seq;
}

TEST(Builder, BuildImmutableList) {
    Builder<int> builder;
    builder.Add(50).Add(60);
    
    ImmutableListSequence<int>* seq = builder.BuildImmutableList();
    EXPECT_EQ(seq->GetLength(), 2);
    delete seq;
}

TEST(Builder, BuildEmpty) {
    Builder<int> builder;
    MutableArraySequence<int>* seq = builder.BuildMutableArray();
    EXPECT_EQ(seq->GetLength(), 0);
    delete seq;
}