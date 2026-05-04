#pragma once

#include "array_sequence.hpp"
#include "list_sequence.hpp"

namespace lab2 {

template <class T>
class Builder {
 public:
  Builder& Add(const T& value) {
    items_.Append(value);
    return *this;
  }

  Builder& AddRange(const LinkedList<T>& values) {
    for (size_t i = 0; i < values.GetLength(); ++i) {
      items_.Append(values.Get(i));
    }
    return *this;
  }

  MutableArraySequence<T>* BuildMutableArray() const {
    const size_t size = items_.GetLength();
    T* data = new T[size];
    for (size_t i = 0; i < size; ++i) {
      data[i] = items_.Get(i);
    }
    MutableArraySequence<T>* result = new MutableArraySequence<T>(data, size);
    delete[] data;
    return result;
  }

  ImmutableArraySequence<T>* BuildImmutableArray() const {
    const size_t size = items_.GetLength();
    T* data = new T[size];
    for (size_t i = 0; i < size; ++i) {
      data[i] = items_.Get(i);
    }
    ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(data, size);
    delete[] data;
    return result;
  }

  MutableListSequence<T>* BuildMutableList() const {
    const size_t size = items_.GetLength();
    T* data = new T[size];
    for (size_t i = 0; i < size; ++i) {
      data[i] = items_.Get(i);
    }
    MutableListSequence<T>* result = new MutableListSequence<T>(data, size);
    delete[] data;
    return result;
  }

  ImmutableListSequence<T>* BuildImmutableList() const {
    const size_t size = items_.GetLength();
    T* data = new T[size];
    for (size_t i = 0; i < size; ++i) {
      data[i] = items_.Get(i);
    }
    ImmutableListSequence<T>* result = new ImmutableListSequence<T>(data, size);
    delete[] data;
    return result;
  }

 private:
  LinkedList<T> items_;
};

} 