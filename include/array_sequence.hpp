#pragma once

#include <algorithm>

#include "dynamic_array.hpp"
#include "linked_list.hpp"
#include "sequence.hpp"

namespace lab2 {

template <class T>
class ArraySequence : public Sequence<T> {
 public:
  ArraySequence() : items_() {
  }

  ArraySequence(const T* items, int count) : items_(items, count) {
  }

  explicit ArraySequence(const LinkedList<T>& list) : items_(list.GetLength()) {
    for (int i = 0; i < list.GetLength(); ++i) {
      items_.Set(i, list.Get(i));
    }
  }

  ArraySequence(const ArraySequence<T>& other) : items_(other.items_) {
  }

  T GetFirst() const override {
    if (GetLength() == 0) {
      throw EmptyCollection("Ошибка, массив пустой");
    }
    return items_.Get(0);
  }

  T GetLast() const override {
    if (GetLength() == 0) {
      throw EmptyCollection("Ошибка, массив пустой");
    }
    return items_.Get(GetLength() - 1);
  }

  T Get(int index) const override {
    return items_.Get(index);
  }

  int GetLength() const override {
    return items_.GetSize();
  }

  Sequence<T>* GetSubsequence(int start_index, int end_index) const override {
    ValidateClosedRange(start_index, end_index);
    ArraySequence<T>* result = CreateConcreteEmpty();
    for (int i = start_index; i <= end_index; ++i) {
      result->AppendInternal(items_.Get(i));
    }
    return result;
  }

}

}