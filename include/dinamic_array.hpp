#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

#include "exception.hpp"
#include "icollection.hpp"

namespace lab2 {

template <class T>
class DynamicArray : public ICollection<T> {
 public:
  DynamicArray() : size_(0), data_(nullptr) {
  }

  explicit DynamicArray(size_t size) : size_(0), data_(nullptr) {
    Allocate(size);
  }

  DynamicArray(const T* items, size_t count) : size_(0), data_(nullptr) {
    Allocate(count);
    for (size_t i = 0; i < count; ++i) {
      data_[i] = items[i];
    }
  }

  DynamicArray(const DynamicArray<T>& other) : size_(0), data_(nullptr) {
    Allocate(other.size_);
    for (size_t i = 0; i < size_; ++i) {
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

  T Get(size_t index) const {
    ValidateIndex(index);
    return data_[index];
  }

  size_t GetSize() const {
    return size_;
  }

  std::size_t GetCount() const override {
    return static_cast<std::size_t>(size_);
  }

  void Set(size_t index, const T& value) {
    ValidateIndex(index);
    data_[index] = value;
  }

  void Resize(size_t new_size) {

    std::unique_ptr<T[]> new_data = (new_size == 0 ? nullptr : std::make_unique<T[]>(new_size));
    const size_t copy_count = std::min(size_, new_size);
    for (size_t i = 0; i < copy_count; ++i) {
      new_data[i] = data_[i];
    }
    for (size_t i = copy_count; i < new_size; ++i) {
      new_data[i] = T();
    }
    data_ = std::move(new_data);
    size_ = new_size;
  }

  const T& operator[](size_t index) const {
    ValidateIndex(index);
    return data_[index];
  }

  T& operator[](size_t index) {
    ValidateIndex(index);
    return data_[index];
  }

 private:
  size_t size_;
  std::unique_ptr<T[]> data_;

  void Allocate(size_t size) {
    size_ = size;
    data_ = (size == 0 ? nullptr : std::make_unique<T[]>(size));
    for (size_t i = 0; i < size_; ++i) {
      data_[i] = T();
    }
  }

  void ValidateIndex(size_t index) const {
    if (index >= size_) {
      throw IndexOutOfRange("Ошибка, индекс не в диапазоне");
    }
  }

  void Swap(DynamicArray<T>& other) noexcept {
    std::swap(size_, other.size_);
    std::swap(data_, other.data_);
  }
};

}
