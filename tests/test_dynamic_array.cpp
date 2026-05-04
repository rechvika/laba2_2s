#include <gtest/gtest.h>
#include "dinamic_array.hpp"

using namespace lab2;

TEST(DynamicArrayConstructor, DefaultConstructor) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArrayConstructor, SizeConstructor) {
    DynamicArray<int> arr(5);
    EXPECT_EQ(arr.GetSize(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arr.Get(i), 0);
    }
}

TEST(DynamicArrayConstructor, SizeConstructorNegative) {
    EXPECT_THROW(DynamicArray<int> arr(-1), InvalidArgument);
}

TEST(DynamicArrayConstructor, SizeConstructorZero) {
    DynamicArray<int> arr(0);
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArrayConstructor, ArrayConstructor) {
    int items[] = {10, 20, 30, 40};
    DynamicArray<int> arr(items, 4);
    EXPECT_EQ(arr.GetSize(), 4);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    EXPECT_EQ(arr.Get(2), 30);
    EXPECT_EQ(arr.Get(3), 40);
}

TEST(DynamicArrayConstructor, ArrayConstructorNullPointer) {
    EXPECT_THROW(DynamicArray<int> arr(nullptr, 5), InvalidArgument);
}

TEST(DynamicArrayConstructor, ArrayConstructorNegativeCount) {
    int items[] = {1, 2, 3};
    EXPECT_THROW(DynamicArray<int> arr(items, -1), InvalidArgument);
}

TEST(DynamicArrayConstructor, ArrayConstructorZeroCount) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr(items, 0);
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArrayConstructor, CopyConstructor) {
    int items[] = {5, 6, 7, 8};
    DynamicArray<int> original(items, 4);
    DynamicArray<int> copy(original);
    
    EXPECT_EQ(copy.GetSize(), 4);
    EXPECT_EQ(copy.Get(0), 5);
    EXPECT_EQ(copy.Get(3), 8);
    
    original.Set(0, 100);
    EXPECT_EQ(copy.Get(0), 5);
}

TEST(DynamicArrayConstructor, CopyConstructorEmpty) {
    DynamicArray<int> original;
    DynamicArray<int> copy(original);
    EXPECT_EQ(copy.GetSize(), 0);
}

TEST(DynamicArrayAssignment, SelfAssignment) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr(items, 3);
    arr = arr;
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(2), 3);
}

TEST(DynamicArrayAssignment, AssignmentOperator) {
    int items1[] = {1, 2, 3};
    int items2[] = {4, 5, 6, 7};
    DynamicArray<int> arr1(items1, 3);
    DynamicArray<int> arr2(items2, 4);
    
    arr1 = arr2;
    EXPECT_EQ(arr1.GetSize(), 4);
    EXPECT_EQ(arr1.Get(0), 4);
    EXPECT_EQ(arr1.Get(3), 7);
}

TEST(DynamicArrayAssignment, AssignmentToEmpty) {
    DynamicArray<int> arr1;
    int items[] = {1, 2, 3};
    DynamicArray<int> arr2(items, 3);
    
    arr1 = arr2;
    EXPECT_EQ(arr1.GetSize(), 3);
    EXPECT_EQ(arr1.Get(0), 1);
}

TEST(DynamicArrayAssignment, AssignmentFromEmpty) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr1(items, 3);
    DynamicArray<int> arr2;
    
    arr1 = arr2;
    EXPECT_EQ(arr1.GetSize(), 0);
}

TEST(DynamicArrayGet, GetValidIndex) {
    int items[] = {10, 20, 30};
    DynamicArray<int> arr(items, 3);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    EXPECT_EQ(arr.Get(2), 30);
}

TEST(DynamicArrayGet, GetInvalidIndexNegative) {
    DynamicArray<int> arr(5);
    EXPECT_THROW(arr.Get(-1), IndexOutOfRange);
}

TEST(DynamicArrayGet, GetInvalidIndexTooLarge) {
    DynamicArray<int> arr(5);
    EXPECT_THROW(arr.Get(5), IndexOutOfRange);
    EXPECT_THROW(arr.Get(100), IndexOutOfRange);
}

TEST(DynamicArrayGet, GetFromEmpty) {
    DynamicArray<int> arr;
    EXPECT_THROW(arr.Get(0), IndexOutOfRange);
}

TEST(DynamicArrayGet, GetSizeTIndex) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr(items, 3);
    std::size_t idx = 1;
    EXPECT_EQ(arr.Get(idx), 2);
}

TEST(DynamicArraySet, SetValidIndex) {
    DynamicArray<int> arr(3);
    arr.Set(1, 42);
    EXPECT_EQ(arr.Get(1), 42);
}

TEST(DynamicArraySet, SetAllIndices) {
    DynamicArray<int> arr(5);
    for (size_t i = 0; i < 5; ++i) {
        arr.Set(i, i * 10);
    }
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arr.Get(i), i * 10);
    }
}

TEST(DynamicArraySet, SetInvalidIndexNegative) {
    DynamicArray<int> arr(3);
    EXPECT_THROW(arr.Set(-1, 10), IndexOutOfRange);
}

TEST(DynamicArraySet, SetInvalidIndexTooLarge) {
    DynamicArray<int> arr(3);
    EXPECT_THROW(arr.Set(3, 10), IndexOutOfRange);
    EXPECT_THROW(arr.Set(100, 10), IndexOutOfRange);
}

TEST(DynamicArraySet, SetOnEmpty) {
    DynamicArray<int> arr;
    EXPECT_THROW(arr.Set(0, 10), IndexOutOfRange);
}

TEST(DynamicArrayResize, ResizeLarger) {
    DynamicArray<int> arr(3);
    arr.Set(0, 1);
    arr.Set(1, 2);
    arr.Set(2, 3);
    
    arr.Resize(5);
    EXPECT_EQ(arr.GetSize(), 5);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(1), 2);
    EXPECT_EQ(arr.Get(2), 3);
    EXPECT_EQ(arr.Get(3), 0);
    EXPECT_EQ(arr.Get(4), 0);
}

TEST(DynamicArrayResize, ResizeSmaller) {
    DynamicArray<int> arr(5);
    for (size_t i = 0; i < 5; ++i) {
        arr.Set(i, i * 10);
    }
    
    arr.Resize(3);
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 0);
    EXPECT_EQ(arr.Get(1), 10);
    EXPECT_EQ(arr.Get(2), 20);
}

TEST(DynamicArrayResize, ResizeToZero) {
    DynamicArray<int> arr(5);
    arr.Resize(0);
    EXPECT_EQ(arr.GetSize(), 0);
    EXPECT_THROW(arr.Get(0), IndexOutOfRange);
}

TEST(DynamicArrayResize, ResizeFromEmpty) {
    DynamicArray<int> arr;
    arr.Resize(5);
    EXPECT_EQ(arr.GetSize(), 5);
    for (size_t i = 0; i < 5; ++i) {
        EXPECT_EQ(arr.Get(i), 0);
    }
}

TEST(DynamicArrayResize, ResizeNegative) {
    DynamicArray<int> arr(5);
    EXPECT_THROW(arr.Resize(-1), InvalidArgument);
}

TEST(DynamicArrayResize, ResizeSameSize) {
    DynamicArray<int> arr(3);
    arr.Set(0, 10);
    arr.Set(1, 20);
    arr.Set(2, 30);
    
    arr.Resize(3);
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    EXPECT_EQ(arr.Get(2), 30);
}

TEST(DynamicArrayResize, MultipleResizes) {
    DynamicArray<int> arr(2);
    arr.Set(0, 1);
    arr.Set(1, 2);
    
    arr.Resize(4);
    arr.Set(2, 3);
    arr.Set(3, 4);
    
    arr.Resize(3);
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(1), 2);
    EXPECT_EQ(arr.Get(2), 3);
    
    arr.Resize(5);
    EXPECT_EQ(arr.Get(4), 0);
}

TEST(DynamicArrayBracket, ConstBracketOperator) {
    int items[] = {100, 200, 300};
    const DynamicArray<int> arr(items, 3);
    EXPECT_EQ(arr[0], 100);
    EXPECT_EQ(arr[1], 200);
    EXPECT_EQ(arr[2], 300);
}

TEST(DynamicArrayBracket, MutableBracketOperator) {
    DynamicArray<int> arr(3);
    arr[0] = 5;
    arr[1] = 6;
    arr[2] = 7;
    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 6);
    EXPECT_EQ(arr[2], 7);
}

TEST(DynamicArrayBracket, ChainedBracketOperations) {
    DynamicArray<int> arr(3);
    arr[0] = arr[1] = arr[2] = 10;
    EXPECT_EQ(arr[0], 10);
    EXPECT_EQ(arr[1], 10);
    EXPECT_EQ(arr[2], 10);
}

TEST(DynamicArrayBracket, ConstBracketInvalidIndex) {
    const DynamicArray<int> arr(2);
    EXPECT_THROW(arr[2], IndexOutOfRange);
}

TEST(DynamicArrayBracket, MutableBracketInvalidIndex) {
    DynamicArray<int> arr(2);
    EXPECT_THROW(arr[2] = 10, IndexOutOfRange);
}

TEST(DynamicArrayICollection, GetCount) {
    int items[] = {1, 2, 3, 4, 5};
    DynamicArray<int> arr(items, 5);
    EXPECT_EQ(arr.GetCount(), 5);
}

TEST(DynamicArrayICollection, GetCountEmpty) {
    DynamicArray<int> arr;
    EXPECT_EQ(arr.GetCount(), 0);
}

TEST(DynamicArrayICollection, GetSizeT) {
    int items[] = {10, 20};
    DynamicArray<int> arr(items, 2);
    EXPECT_EQ(arr.Get(static_cast<std::size_t>(0)), 10);
    EXPECT_EQ(arr.Get(static_cast<std::size_t>(1)), 20);
}

TEST(DynamicArrayType, StringType) {
    DynamicArray<std::string> arr(3);
    arr.Set(0, "привет");
    arr.Set(1, "спанчбоб");
    arr.Set(2, "!");
    EXPECT_EQ(arr.Get(0), "привет");
    EXPECT_EQ(arr.Get(1), "спанчбоб");
    EXPECT_EQ(arr.Get(2), "!");
}

TEST(DynamicArrayType, DoubleType) {
    double items[] = {1.1, 2.2, 3.3};
    DynamicArray<double> arr(items, 3);
    EXPECT_DOUBLE_EQ(arr.Get(0), 1.1);
    EXPECT_DOUBLE_EQ(arr.Get(1), 2.2);
    EXPECT_DOUBLE_EQ(arr.Get(2), 3.3);
}

TEST(DynamicArrayType, BoolType) {
    bool items[] = {true, false, true};
    DynamicArray<bool> arr(items, 3);
    EXPECT_EQ(arr.Get(0), true);
    EXPECT_EQ(arr.Get(1), false);
    EXPECT_EQ(arr.Get(2), true);
}

TEST(DynamicArrayPerformance, ManyResizes) {
    DynamicArray<int> arr;
    for (int i = 0; i < 1000; ++i) {
        arr.Resize(i + 1);
        arr.Set(i, i);
    }
    EXPECT_EQ(arr.GetSize(), 1000);
    EXPECT_EQ(arr.Get(500), 500);
}

TEST(DynamicArrayPerformance, ManySetsAndGets) {
    DynamicArray<int> arr(1000);
    for (int i = 0; i < 1000; ++i) {
        arr.Set(i, i * 2);
    }
    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(arr.Get(i), i * 2);
    }
}