#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

#include "exceptions.hpp"
#include "icollection.hpp"

namespace lab2 {

template <class T>
class DynamicArray : public ICollection<T> {
 public:
  DynamicArray() : size_(0), data_(nullptr) {
  }

  explicit DynamicArray(int size) : size_(0), data_(nullptr) {
    if (size < 0) {
      throw InvalidArgument("Ошибка, отрицатльный размер");
    }
    Allocate(size);
  }

  DynamicArray(const T* items, int count) : size_(0), data_(nullptr) {
    if (count < 0) {
      throw InvalidArgument("Ошибка, отрицатльный размер");
    }
    Allocate(count);
    for (int i = 0; i < count; ++i) {
      data_[i] = items[i];
    }
  }

  DynamicArray(const DynamicArray<T>& other) : size_(0), data_(nullptr) {
    Allocate(other.size_);
    for (int i = 0; i < size_; ++i) {
      data_[i] = other.data_[i];
    }
  }

  DynamicArray<T>& operator=(const DynamicArray<T>& other) {
    if (this == &other) {
      return *this;
    }
    DynamicArray<T> copy(other);
    Swap(copy);
    return *this;
  }

  T Get(int index) const {
    ValidateIndex(index);
    return data_[index];
  }

  T Get(std::size_t index) const override {
    if (index > static_cast<std::size_t>(static_cast<int>(index))) {
      throw IndexOutOfRange("Ошибка, слишком большой индекс");
    }
    return Get(static_cast<int>(index));
  }

  int GetSize() const {
    return size_;
  }

  std::size_t GetCount() const override {
    return static_cast<std::size_t>(size_);
  }

  void Set(int index, const T& value) {
    ValidateIndex(index);
    data_[index] = value;
  }

  void Resize(int new_size) {
    if (new_size < 0) {
      throw InvalidArgument("Ошибка, отрицательный размер");
    }
    std::unique_ptr<T[]> new_data = (new_size == 0 ? nullptr : std::make_unique<T[]>(new_size));
    const int copy_count = std::min(size_, new_size);
    for (int i = 0; i < copy_count; ++i) {
      new_data[i] = data_[i];
    }
    for (int i = copy_count; i < new_size; ++i) {
      new_data[i] = T();
    }
    data_ = std::move(new_data);
    size_ = new_size;
  }

  const T& operator[](int index) const {
    ValidateIndex(index);
    return data_[index];
  }

  T& operator[](int index) {
    ValidateIndex(index);
    return data_[index];
  }

 private:
  int size_;
  std::unique_ptr<T[]> data_;

  void Allocate(int size) {
    size_ = size;
    data_ = (size == 0 ? nullptr : std::make_unique<T[]>(size));
    for (int i = 0; i < size_; ++i) {
      data_[i] = T();
    }
  }

  void ValidateIndex(int index) const {
    if (index < 0 || index >= size_) {
      throw IndexOutOfRange("Ошибка, индекс не в диапазоне");
    }
  }

  void Swap(DynamicArray<T>& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
  }
};

}
