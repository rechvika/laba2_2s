#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>

#include "exception.hpp"
#include "icollection.hpp"
#include "ienumerator.hpp"

namespace lab2 {

template <class T>
class DynamicArray : public ICollection<T> {
  static constexpr size_t max_size_ = 1024 * 1024 * 100;
 public:
  DynamicArray() : size_(0), data_(nullptr) {
  }

  explicit DynamicArray(size_t size) : size_(0), data_(nullptr) {
    Allocate(size);
  }

  DynamicArray(const T* items, size_t count) : DynamicArray() {
    if (items == nullptr && count > 0) {
      throw InvalidArgument("Ошибка, указатель на массив нулевой");
    }
    Allocate(count);
    for (size_t i = 0; i < count; ++i) {
      data_[i] = items[i];
    }
  }

  DynamicArray(const DynamicArray<T>& other) : DynamicArray() {
    Allocate(other.size_);
    for (size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

  DynamicArray<T>& operator=(const DynamicArray<T>& other) {
    if (this != &other) {
        Resize(other.size_);
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
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
    return size_;
  }

  T* Set(size_t index, const T& value) {
    ValidateIndex(index);
    data_[index] = value;
    return &data_[index];
  }

  void Resize(size_t new_size) {
    if (new_size > max_size_) {
        throw InvalidArgument("Ошибка, размер " + std::to_string(new_size) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(max_size_));
    }
    std::unique_ptr<T[]> new_data;
    if (new_size > 0) {
        new_data = std::make_unique<T[]>(new_size);
    }
    const size_t copy_count = std::min(size_, new_size);
    for (size_t i = 0; i < copy_count; ++i) {
        if (data_) {
            new_data[i] = data_[i];
        }
    }
    data_ = std::move(new_data);
    size_ = new_size;
}

  const T& operator[](size_t index) const {
    return data_[index];
  }

  T& operator[](size_t index) {
    return data_[index];
  }

  class Enumerator : public IEnumerator<T> {
  public:
    explicit Enumerator(const DynamicArray<T>& array) 
        : array_(array), index_(0), current_() {}
    
    bool MoveNext() override {
      if (index_ >= array_.GetSize()) {
        return false;
      }
      current_ = array_.Get(index_);
      ++index_;
      return true;
    }
    
    T Current() const override {
      if (index_ == 0) {
        throw InvalidArgument("Ошибка, вызов Current() до MoveNext()");
      }
      if (index_ - 1 >= array_.GetSize()) {
        throw IndexOutOfRange("Ошибка, итератор вышел за пределы");
      }
      return current_;
    }
    
    void Reset() override {
      index_ = 0;
      current_ = T();
    }
    
  private:
    const DynamicArray<T>& array_;
    size_t index_;
    T current_;
  };
  
  std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
    return std::make_unique<Enumerator>(*this);
  }

 private:
  size_t size_;
  std::unique_ptr<T[]> data_;

  void Allocate(size_t size) {
    if (size > max_size_) {
        throw InvalidArgument("Ошибка, размер " + std::to_string(size) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(max_size_));
    }
    if (size == 0) {
        data_ = nullptr;
        size_ = 0;
        return;
    }
    data_ = std::make_unique<T[]>(size);
    size_ = size;
  }

  void ValidateIndex(size_t index) const {
    if (index >= size_) {
      throw IndexOutOfRange("Ошибка, индекс " + std::to_string(index) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(size_ - 1));
    }
  }

  void Swap(DynamicArray<T>& other) noexcept {
    size_t temp_size = size_;
    size_ = other.size_;
    other.size_ = temp_size;
    
    std::unique_ptr<T[]> temp_data = std::move(data_);
    data_ = std::move(other.data_);
    other.data_ = std::move(temp_data);
  }
};

}