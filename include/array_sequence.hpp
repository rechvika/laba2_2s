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

  Sequence<T>* Append(const T& item) override {
    return PrepareForWrite()->AppendInternal(item);
  }

  Sequence<T>* Prepend(const T& item) override {
    return PrepareForWrite()->PrependInternal(item);
  }

  Sequence<T>* InsertAt(const T& item, int index) override {
    if (index < 0 || index >= GetLength()) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    return PrepareForWrite()->InsertAtInternal(item, index);
  }

  Sequence<T>* Set(int index, const T& item) override {
    return PrepareForWrite()->SetInternal(index, item);
  }

  Sequence<T>* Slice(int index, int count, const Sequence<T>* replacement = nullptr) override {
    return PrepareForWrite()->SliceInternal(index, count, replacement);
  }

  Sequence<T>* Concat(const Sequence<T>& other) override {
    return PrepareForWrite()->ConcatInternal(other);
  }

 protected:
  DynamicArray<T> items_;

  virtual ArraySequence<T>* PrepareForWrite() = 0;
  virtual ArraySequence<T>* CreateConcreteEmpty() const = 0;

  ArraySequence<T>* AppendInternal(const T& item) {
    items_.Resize(items_.GetSize() + 1);
    items_.Set(items_.GetSize() - 1, item);
    return this;
  }

  ArraySequence<T>* PrependInternal(const T& item) {
    items_.Resize(items_.GetSize() + 1);
    for (int i = items_.GetSize() - 1; i > 0; --i) {
      items_.Set(i, items_.Get(i - 1));
    }
    items_.Set(0, item);
    return this;
  }

  ArraySequence<T>* InsertAtInternal(const T& item, int index) {
    items_.Resize(items_.GetSize() + 1);
    for (int i = items_.GetSize() - 1; i > index; --i) {
      items_.Set(i, items_.Get(i - 1));
    }
    items_.Set(index, item);
    return this;
  }

  ArraySequence<T>* SetInternal(int index, const T& item) {
    items_.Set(index, item);
    return this;
  }

  ArraySequence<T>* SliceInternal(int index, int count, const Sequence<T>* replacement) {
    if (count < 0) {
      throw InvalidArgument("Ошибка, отрицательный размер");
    }
    const int start = NormalizeSliceIndex(index);
    const int available = GetLength() - start;
    const int remove_count = std::min(count, available);
    const int replacement_length = (replacement == nullptr ? 0 : replacement->GetLength());
    DynamicArray<T> rebuilt(GetLength() - remove_count + replacement_length);
    int out = 0;
    for (int i = 0; i < start; ++i) {
      rebuilt.Set(out++, items_.Get(i));
    }
    if (replacement != nullptr) {
      for (int i = 0; i < replacement->GetLength(); ++i) {
        rebuilt.Set(out++, replacement->Get(i));
      }
    }
    for (int i = start + remove_count; i < GetLength(); ++i) {
      rebuilt.Set(out++, items_.Get(i));
    }
    items_ = rebuilt;
    return this;
  }

  ArraySequence<T>* ConcatInternal(const Sequence<T>& other) {
    const int old_length = GetLength();
    items_.Resize(old_length + other.GetLength());
    for (int i = 0; i < other.GetLength(); ++i) {
      items_.Set(old_length + i, other.Get(i));
    }
    return this;
  }

  void ValidateClosedRange(int start_index, int end_index) const {
    if (GetLength() == 0) {
      throw EmptyCollection("Ошибка, объект пустой");
    }
    if (start_index < 0 || end_index < 0 || start_index >= GetLength() || end_index >= GetLength()) {
      throw IndexOutOfRange("Ошибка, индексы не входят в допустимый диапазон");
    }
    if (start_index > end_index) {
      throw InvalidArgument("Ошибка, стартовый индекс больше конечного индекса");
    }
  }

  int NormalizeSliceIndex(int index) const {
    int normalized = index;
    if (normalized < 0) {
      normalized = GetLength() + normalized;
    }
    if (normalized < 0 || normalized > GetLength()) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    return normalized;
  }
};

template <class T>
class MutableArraySequence : public ArraySequence<T> {
 public:
  MutableArraySequence() = default;

  MutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) {
  }

  explicit MutableArraySequence(const LinkedList<T>& list) : ArraySequence<T>(list) {
  }

  MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) {
  }

  Sequence<T>* Clone() const override {
    return new MutableArraySequence<T>(*this);
  }

  Sequence<T>* CreateEmpty() const override {
    return new MutableArraySequence<T>();
  }

  const char* StorageName() const override {
    return "MutableArraySequence";
  }

 protected:
  ArraySequence<T>* PrepareForWrite() override {
    return this;
  }

  ArraySequence<T>* CreateConcreteEmpty() const override {
    return new MutableArraySequence<T>();
  }
};

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
 public:
  ImmutableArraySequence() = default;

  ImmutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) {
  }

  explicit ImmutableArraySequence(const LinkedList<T>& list) : ArraySequence<T>(list) {
  }

  ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(other) {
  }

  Sequence<T>* Clone() const override {
    return new ImmutableArraySequence<T>(*this);
  }

  Sequence<T>* CreateEmpty() const override {
    return new ImmutableArraySequence<T>();
  }

  const char* StorageName() const override {
    return "ImmutableArraySequence";
  }

 protected:
  ArraySequence<T>* PrepareForWrite() override {
    return new ImmutableArraySequence<T>(*this);
  }

  ArraySequence<T>* CreateConcreteEmpty() const override {
    return new ImmutableArraySequence<T>();
  }
};

}