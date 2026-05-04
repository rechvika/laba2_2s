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
TEST(LinkedListSet, ValidIndex) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);
    list.Set(1, 99);
    EXPECT_EQ(list.Get(1), 99);
    list.Set(0, 100);
    EXPECT_EQ(list.Get(0), 100);
    list.Set(2, 300);
    EXPECT_EQ(list.Get(2), 300);
}

TEST(LinkedListSet, InvalidIndex) {
    LinkedList<int> list;
    list.Append(1);
    EXPECT_THROW(list.Set(-1, 10), IndexOutOfRange);
    EXPECT_THROW(list.Set(1, 10), IndexOutOfRange);
    EXPECT_THROW(LinkedList<int>().Set(0, 10), IndexOutOfRange);
}

TEST(LinkedListAppend, Normal) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListPrepend, Normal) {
    LinkedList<int> list;
    list.Prepend(3);
    list.Prepend(2);
    list.Prepend(1);
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListInsertAt, Normal) {
    LinkedList<int> list;
    list.Append(2);
    list.Append(3);
    list.InsertAt(1, 0);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
    
    list.InsertAt(5, 3);
    EXPECT_EQ(list.Get(3), 5);
}

TEST(LinkedListInsertAt, InvalidIndex) {
    LinkedList<int> list;
    list.Append(1);
    EXPECT_THROW(list.InsertAt(5, -1), IndexOutOfRange);
    EXPECT_THROW(list.InsertAt(5, 2), IndexOutOfRange);
    EXPECT_THROW(LinkedList<int>().InsertAt(1, 0), IndexOutOfRange);
}

TEST(LinkedListGetSubList, Normal) {
    int items[] = {10, 20, 30, 40, 50};
    LinkedList<int> list(items, 5);
    LinkedList<int>* sublist = list.GetSubList(1, 3);
    
    EXPECT_EQ(sublist->GetLength(), 3);
    EXPECT_EQ(sublist->Get(0), 20);
    EXPECT_EQ(sublist->Get(1), 30);
    EXPECT_EQ(sublist->Get(2), 40);
    
    delete sublist;
}

TEST(LinkedListGetSubList, InvalidRange) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);
    EXPECT_THROW(list.GetSubList(2, 1), InvalidArgument);
    EXPECT_THROW(list.GetSubList(0, 5), IndexOutOfRange);
    EXPECT_THROW(list.GetSubList(5, 6), IndexOutOfRange);
    EXPECT_THROW(LinkedList<int>().GetSubList(0, 0), IndexOutOfRange);
}

TEST(LinkedListConcat, Normal) {
    int items1[] = {1, 2, 3};
    int items2[] = {4, 5, 6};
    LinkedList<int> list1(items1, 3);
    LinkedList<int> list2(items2, 3);
    
    LinkedList<int>* result = list1.Concat(&list2);
    EXPECT_EQ(result->GetLength(), 6);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(5), 6);
    
    delete result;
}

TEST(LinkedListConcat, WithEmpty) {
    int items[] = {1, 2, 3};
    LinkedList<int> list1(items, 3);
    LinkedList<int> list2;
    
    LinkedList<int>* result = list1.Concat(&list2);
    EXPECT_EQ(result->GetLength(), 3);
    delete result;
}

TEST(LinkedListConcat, NullOther) {
    LinkedList<int> list;
    list.Append(1);
    EXPECT_THROW(list.Concat(nullptr), InvalidArgument);
}

TEST(LinkedListDestructor, DoesNotCrash) {
    LinkedList<int>* list = new LinkedList<int>();
    delete list;
    
    list = new LinkedList<int>();
    for (int i = 0; i < 100; ++i) list->Append(i);
    delete list;
}

TEST(LinkedListICollection, GetCount) {
    int items[] = {1, 2, 3, 4, 5};
    LinkedList<int> list(items, 5);
    EXPECT_EQ(list.GetCount(), 5);
    EXPECT_EQ(LinkedList<int>().GetCount(), 0);
}

TEST(LinkedListEdgeCases, StringType) {
    LinkedList<std::string> list;
    list.Append("привет");
    list.Prepend("спанчбоб");
    list.InsertAt("!", 1);
    EXPECT_EQ(list.Get(0), "спанчбоб");
    EXPECT_EQ(list.Get(1), "!");
    EXPECT_EQ(list.Get(2), "привет");
}