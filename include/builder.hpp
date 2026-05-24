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
    auto enumerator = values.GetEnumerator();
    while (enumerator->MoveNext()) {
        items_.Append(enumerator->Current());
    }
    return *this;
}
  MutableArraySequence<T>* BuildMutableArray() const {
    const size_t size = items_.GetLength();
    T* data = new T[size];
    size_t index = 0;
    auto enumerator = items_.GetEnumerator();
    while (enumerator->MoveNext()) {
        data[index++] = enumerator->Current();
    }
    MutableArraySequence<T>* result = new MutableArraySequence<T>(data, size);
    delete[] data;
    return result;
}

  ImmutableArraySequence<T>* BuildImmutableArray() const {
    const size_t size = items_.GetLength();
    T* data = new T[size];
    size_t index = 0;
    auto enumerator = items_.GetEnumerator();
    while (enumerator->MoveNext()) {
        data[index++] = enumerator->Current();
    }
    ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(data, size);
    delete[] data;
    return result;
}

  MutableListSequence<T>* BuildMutableList() const {
    const size_t size = items_.GetLength();
    T* data = new T[size];
    size_t index = 0;
    auto enumerator = items_.GetEnumerator();
    while (enumerator->MoveNext()) {
        data[index++] = enumerator->Current();
    }
    MutableListSequence<T>* result = new MutableListSequence<T>(data, size);
    delete[] data;
    return result;
}

  ImmutableListSequence<T>* BuildImmutableList() const {
    const size_t size = items_.GetLength();
    T* data = new T[size];
    size_t index = 0;
    auto enumerator = items_.GetEnumerator();
    while (enumerator->MoveNext()) {
        data[index++] = enumerator->Current();
    }
    ImmutableListSequence<T>* result = new ImmutableListSequence<T>(data, size);
    delete[] data;
    return result;
}

 private:
  LinkedList<T> items_;
};

} 