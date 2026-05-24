#pragma once

#include <algorithm>

#include "dinamic_array.hpp"
#include "linked_list.hpp"
#include "sequence.hpp"

namespace lab2 {

template <class T>
class ArraySequence : public Sequence<T> {
 public:
  ArraySequence() : items_() {
  }

  ArraySequence(const T* items, size_t count) : items_(items, count) {
  }

  template <class Container>
  explicit ArraySequence(const Container& container) : items_(container.GetLength()) {
    auto enumerator = container.GetEnumerator();
    size_t i = 0;
    while (enumerator->MoveNext()) {
      items_.Set(i++, enumerator->Current());
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

  T Get(size_t index) const override {
    return items_.Get(index);
  }

  size_t GetLength() const override {
    return items_.GetSize();
  }

  Sequence<T>* GetSubsequence(size_t start_index, size_t end_index) const override {
    ValidateClosedRange(start_index, end_index);
    ArraySequence<T>* result = CreateConcreteEmpty();
    for (size_t i = start_index; i <= end_index; ++i) {
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

  Sequence<T>* InsertAt(const T& item, size_t index) override {
    if (index >= GetLength()) {
      throw IndexOutOfRange("Ошибка, индекс " + std::to_string(index) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(GetLength() - 1));
    }
    return PrepareForWrite()->InsertAtInternal(item, index);
  }

  Sequence<T>* Set(size_t index, const T& item) override {
    return PrepareForWrite()->SetInternal(index, item);
  }

  Sequence<T>* Slice(size_t index, size_t count, const Sequence<T>* replacement = nullptr) override {
    if (count > GetLength() - index) {
        throw InvalidArgument("Ошибка, количество элементов для удаления " + std::to_string(count) + 
                              " больше доступного количества " + std::to_string(GetLength() - index));
    }
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
    for (size_t i = items_.GetSize() - 1; i > 0; --i) {
      items_.Set(i, items_.Get(i - 1));
    }
    items_.Set(0, item);
    return this;
  }

  ArraySequence<T>* InsertAtInternal(const T& item, size_t index) {
    items_.Resize(items_.GetSize() + 1);
    for (size_t i = items_.GetSize() - 1; i > index; --i) {
      items_.Set(i, items_.Get(i - 1));
    }
    items_.Set(index, item);
    return this;
  }

  ArraySequence<T>* SetInternal(size_t index, const T& item) {
    items_.Set(index, item);
    return this;
  }

  ArraySequence<T>* SliceInternal(size_t index, size_t count, const Sequence<T>* replacement) {

    const size_t start = NormalizeSliceIndex(index);
    const size_t available = GetLength() - start;
    const size_t remove_count = std::min(count, available);
    const size_t replacement_length = (replacement == nullptr ? 0 : replacement->GetLength());
    DynamicArray<T> rebuilt(GetLength() - remove_count + replacement_length);
    size_t out = 0;
    for (size_t i = 0; i < start; ++i) {
      rebuilt.Set(out++, items_.Get(i));
    }
    if (replacement != nullptr) {
      auto enumerator = replacement->GetEnumerator();
      while (enumerator->MoveNext()) {
        rebuilt.Set(out++, enumerator->Current());
      }
    }
    for (size_t i = start + remove_count; i < GetLength(); ++i) {
      rebuilt.Set(out++, items_.Get(i));
    }
    items_ = rebuilt;
    return this;
  }

  ArraySequence<T>* ConcatInternal(const Sequence<T>& other) {
    const size_t old_length = GetLength();
    items_.Resize(old_length + other.GetLength());
    auto enumerator = other.GetEnumerator();
    size_t i = 0;
    while (enumerator->MoveNext()) {
      items_.Set(old_length + i++, enumerator->Current());
    }
    return this;
  }

  void ValidateClosedRange(size_t start_index, size_t end_index) const {
    if (GetLength() == 0) {
      throw EmptyCollection("Ошибка, объект пустой");
    }
    if (start_index >= GetLength()) {
      throw IndexOutOfRange("Ошибка, стартовый индекс " + std::to_string(start_index) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(GetLength() - 1));
    }
    if (end_index >= GetLength()) {
      throw IndexOutOfRange("Ошибка, конечный индекс " + std::to_string(end_index) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(GetLength() - 1));
    }
    if (start_index > end_index) {
      throw InvalidArgument("Ошибка, стартовый индекс " + std::to_string(start_index) + 
                              " больше конечного индекса " + std::to_string(end_index));
    }
  }

  size_t NormalizeSliceIndex(size_t index) const {
    size_t normalized = index;

    if (normalized > GetLength()) {
      throw IndexOutOfRange("Ошибка, индекс " + std::to_string(index) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(GetLength()));
    }
    return normalized;
  }
};

template <class T>
class MutableArraySequence : public ArraySequence<T> {
 public:
  MutableArraySequence() = default;

  MutableArraySequence(const T* items, size_t count) : ArraySequence<T>(items, count) {
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

  ImmutableArraySequence(const T* items, size_t count) : ArraySequence<T>(items, count) {
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