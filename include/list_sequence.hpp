#pragma once

#include <algorithm>

#include "linked_list.hpp"
#include "sequence.hpp"

namespace lab2 {

template <class T>
class ListSequence : public Sequence<T> {
 public:
  ListSequence() : items_() {
  }

  ListSequence(const T* items, size_t count) : items_(items, count) {
  }

  explicit ListSequence(const LinkedList<T>& list) : items_(list) {
  }

  ListSequence(const ListSequence<T>& other) : items_(other.items_) {
  }

  T GetFirst() const override {
    return items_.GetFirst();
  }

  T GetLast() const override {
    return items_.GetLast();
  }

  T Get(size_t index) const override {
    return items_.Get(index);
  }

  size_t GetLength() const override {
    return items_.GetLength();
  }

  Sequence<T>* GetSubsequence(size_t start_index, size_t end_index) const override {
    ValidateClosedRange(start_index, end_index);
    ListSequence<T>* result = CreateConcreteEmpty();
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
    if (index < 0 || index >= GetLength()) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    return PrepareForWrite()->InsertAtInternal(item, index);
  }

  Sequence<T>* Set(size_t index, const T& item) override {
    return PrepareForWrite()->SetInternal(index, item);
  }

  Sequence<T>* Slice(size_t index, size_t count, const Sequence<T>* replacement = nullptr) override {
    return PrepareForWrite()->SliceInternal(index, count, replacement);
  }

  Sequence<T>* Concat(const Sequence<T>& other) override {
    return PrepareForWrite()->ConcatInternal(other);
  }

 protected:
  LinkedList<T> items_;

  virtual ListSequence<T>* PrepareForWrite() = 0;
  virtual ListSequence<T>* CreateConcreteEmpty() const = 0;

  ListSequence<T>* AppendInternal(const T& item) {
    items_.Append(item);
    return this;
  }

  ListSequence<T>* PrependInternal(const T& item) {
    items_.Prepend(item);
    return this;
  }

  ListSequence<T>* InsertAtInternal(const T& item, size_t index) {
    items_.InsertAt(item, index);
    return this;
  }

  ListSequence<T>* SetInternal(size_t index, const T& item) {
    items_.Set(index, item);
    return this;
  }

  ListSequence<T>* SliceInternal(size_t index, size_t count, const Sequence<T>* replacement) {
    if (count < 0) {
      throw InvalidArgument("Ошибка, отрицательный размер");
    }
    const size_t start = NormalizeSliceIndex(index);
    const size_t available = GetLength() - start;
    const size_t remove_count = std::min(count, available);
    LinkedList<T> rebuilt;
    for (size_t i = 0; i < start; ++i) {
      rebuilt.Append(items_.Get(i));
    }
    if (replacement != nullptr) {
      for (size_t i = 0; i < replacement->GetLength(); ++i) {
        rebuilt.Append(replacement->Get(i));
      }
    }
    for (size_t i = start + remove_count; i < GetLength(); ++i) {
      rebuilt.Append(items_.Get(i));
    }
    items_ = rebuilt;
    return this;
  }

  ListSequence<T>* ConcatInternal(const Sequence<T>& other) {
    for (size_t i = 0; i < other.GetLength(); ++i) {
      items_.Append(other.Get(i));
    }
    return this;
  }

  void ValidateClosedRange(size_t start_index, size_t end_index) const {
    if (GetLength() == 0) {
      throw EmptyCollection("Ошибка, индекс равено 0");
    }
    if (start_index < 0 || end_index < 0 || start_index >= GetLength() || end_index >= GetLength()) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    if (start_index > end_index) {
      throw InvalidArgument("Ошибка, стартовый индекс больше конечного");
    }
  }

  size_t NormalizeSliceIndex(size_t index) const {
    size_t normalized = index;
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
class MutableListSequence : public ListSequence<T> {
 public:
  MutableListSequence() = default;

  MutableListSequence(const T* items, size_t count) : ListSequence<T>(items, count) {
  }

  explicit MutableListSequence(const LinkedList<T>& list) : ListSequence<T>(list) {
  }

  MutableListSequence(const MutableListSequence<T>& other) : ListSequence<T>(other) {
  }

  Sequence<T>* Clone() const override {
    return new MutableListSequence<T>(*this);
  }

  Sequence<T>* CreateEmpty() const override {
    return new MutableListSequence<T>();
  }

  const char* StorageName() const override {
    return "MutableListSequence";
  }

 protected:
  ListSequence<T>* PrepareForWrite() override {
    return this;
  }

  ListSequence<T>* CreateConcreteEmpty() const override {
    return new MutableListSequence<T>();
  }
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
 public:
  ImmutableListSequence() = default;

  ImmutableListSequence(const T* items, size_t count) : ListSequence<T>(items, count) {
  }

  explicit ImmutableListSequence(const LinkedList<T>& list) : ListSequence<T>(list) {
  }

  ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {
  }

  Sequence<T>* Clone() const override {
    return new ImmutableListSequence<T>(*this);
  }

  Sequence<T>* CreateEmpty() const override {
    return new ImmutableListSequence<T>();
  }

  const char* StorageName() const override {
    return "ImmutableListSequence";
  }

 protected:
  ListSequence<T>* PrepareForWrite() override {
    return new ImmutableListSequence<T>(*this);
  }

  ListSequence<T>* CreateConcreteEmpty() const override {
    return new ImmutableListSequence<T>();
  }
};

}
