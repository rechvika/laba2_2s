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

  ListSequence(const T* items, int count) : items_(items, count) {
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

  T Get(int index) const override {
    return items_.Get(index);
  }

  int GetLength() const override {
    return items_.GetLength();
  }

  Sequence<T>* GetSubsequence(int start_index, int end_index) const override {
    ValidateClosedRange(start_index, end_index);
    ListSequence<T>* result = CreateConcreteEmpty();
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

  ListSequence<T>* InsertAtInternal(const T& item, int index) {
    items_.InsertAt(item, index);
    return this;
  }

  ListSequence<T>* SetInternal(int index, const T& item) {
    items_.Set(index, item);
    return this;
  }

  ListSequence<T>* SliceInternal(int index, int count, const Sequence<T>* replacement) {
    if (count < 0) {
      throw InvalidArgument("Ошибка, отрицательный размер");
    }
    const int start = NormalizeSliceIndex(index);
    const int available = GetLength() - start;
    const int remove_count = std::min(count, available);
    LinkedList<T> rebuilt;
    for (int i = 0; i < start; ++i) {
      rebuilt.Append(items_.Get(i));
    }
    if (replacement != nullptr) {
      for (int i = 0; i < replacement->GetLength(); ++i) {
        rebuilt.Append(replacement->Get(i));
      }
    }
    for (int i = start + remove_count; i < GetLength(); ++i) {
      rebuilt.Append(items_.Get(i));
    }
    items_ = rebuilt;
    return this;
  }

  ListSequence<T>* ConcatInternal(const Sequence<T>& other) {
    for (int i = 0; i < other.GetLength(); ++i) {
      items_.Append(other.Get(i));
    }
    return this;
  }

  void ValidateClosedRange(int start_index, int end_index) const {
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
class MutableListSequence : public ListSequence<T> {
 public:
  MutableListSequence() = default;

  MutableListSequence(const T* items, int count) : ListSequence<T>(items, count) {
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

  ImmutableListSequence(const T* items, int count) : ListSequence<T>(items, count) {
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
