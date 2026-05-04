#include <gtest/gtest.h>
#include "linked_list.hpp"

using namespace lab2;

TEST(LinkedListConstructor, DefaultConstructor) {
    LinkedList<int> list;
    EXPECT_EQ(list.GetLength(), 0);
}

TEST(LinkedListConstructor, ArrayConstructor) {
    int items[] = {1, 2, 3, 4, 5};
    LinkedList<int> list(items, 5);
    EXPECT_EQ(list.GetLength(), 5);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(2), 3);
    EXPECT_EQ(list.Get(4), 5);
}

TEST(LinkedListConstructor, ArrayConstructorInvalid) {
    int items[] = {1, 2};
    EXPECT_THROW(LinkedList<int> list(items, -1), InvalidArgument);
    EXPECT_THROW(LinkedList<int> list(nullptr, 5), InvalidArgument);
    
    LinkedList<int> list(items, 0);
    EXPECT_EQ(list.GetLength(), 0);
}

TEST(LinkedListConstructor, CopyConstructor) {
    int items[] = {10, 20, 30};
    LinkedList<int> original(items, 3);
    LinkedList<int> copy(original);
    
    EXPECT_EQ(copy.GetLength(), 3);
    EXPECT_EQ(copy.Get(0), 10);
    EXPECT_EQ(copy.Get(2), 30);
    
    original.Set(0, 100);
    EXPECT_EQ(copy.Get(0), 10);
}

TEST(LinkedListAssignment, AssignmentOperator) {
    int items1[] = {1, 2, 3};
    int items2[] = {4, 5, 6, 7};
    LinkedList<int> list1(items1, 3);
    LinkedList<int> list2(items2, 4);
    
    list1 = list2;
    EXPECT_EQ(list1.GetLength(), 4);
    EXPECT_EQ(list1.Get(0), 4);
    EXPECT_EQ(list1.Get(3), 7);
    
    list1 = list1;
    EXPECT_EQ(list1.GetLength(), 4);
}

TEST(LinkedListGetFirstLast, Normal) {
    int items[] = {5, 10, 15};
    LinkedList<int> list(items, 3);
    EXPECT_EQ(list.GetFirst(), 5);
    EXPECT_EQ(list.GetLast(), 15);
}

TEST(LinkedListGetFirstLast, SingleElement) {
    LinkedList<int> list;
    list.Append(42);
    EXPECT_EQ(list.GetFirst(), 42);
    EXPECT_EQ(list.GetLast(), 42);
}

TEST(LinkedListGetFirstLast, Empty) {
    LinkedList<int> list;
    EXPECT_THROW(list.GetFirst(), EmptyCollection);
    EXPECT_THROW(list.GetLast(), EmptyCollection);
}

TEST(LinkedListGet, ValidIndex) {
    int items[] = {100, 200, 300, 400};
    LinkedList<int> list(items, 4);
    EXPECT_EQ(list.Get(0), 100);
    EXPECT_EQ(list.Get(2), 300);
    EXPECT_EQ(list.Get(3), 400);
}

TEST(LinkedListGet, InvalidIndex) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    EXPECT_THROW(list.Get(-1), IndexOutOfRange);
    EXPECT_THROW(list.Get(2), IndexOutOfRange);
    EXPECT_THROW(list.Get(100), IndexOutOfRange);
    EXPECT_THROW(LinkedList<int>().Get(0), IndexOutOfRange);
}
