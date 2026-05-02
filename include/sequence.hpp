#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "ienumerator.hpp"
#include "option.hpp"

namespace lab2 {

namespace detail {

template <class T>
void TakeResult(std::unique_ptr<T>& owner, T* next) {
  if (next != owner.get()) {
    owner.reset(next);
  }
}

}

template <class T>
class Sequence : public ICollection<T> {
 public:
  ~Sequence() override = default;

  virtual T GetFirst() const = 0;
  virtual T GetLast() const = 0;
  virtual T Get(int index) const = 0;
  T Get(std::size_t index) const override {
    return Get(static_cast<int>(index));
  }
  virtual int GetLength() const = 0;
  std::size_t GetCount() const override {
    return static_cast<std::size_t>(GetLength());
  }

  virtual Sequence<T>* GetSubsequence(int start_index, int end_index) const = 0;
  virtual Sequence<T>* Append(const T& item) = 0;
  virtual Sequence<T>* Prepend(const T& item) = 0;
  virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
  virtual Sequence<T>* Concat(const Sequence<T>& other) = 0;

  virtual Sequence<T>* Set(int index, const T& item) = 0;
  virtual Sequence<T>* Slice(int index, int count, const Sequence<T>* replacement = nullptr) = 0;

  virtual Sequence<T>* Clone() const = 0;
  virtual Sequence<T>* CreateEmpty() const = 0;
  virtual const char* StorageName() const = 0;

  Option<T> TryFirst(const std::function<bool(const T&)>& pred = nullptr) const {
    if (GetLength() == 0) {
      return Option<T>::None();
    }
    if (!pred) {
      return Option<T>(GetFirst());
    }
    for (int i = 0; i < GetLength(); ++i) {
      T value = Get(i);
      if (pred(value)) {
        return Option<T>(value);
      }
    }
    return Option<T>::None();
  }

  Option<T> TryLast(const std::function<bool(const T&)>& pred = nullptr) const {
    if (GetLength() == 0) {
      return Option<T>::None();
    }
    if (!pred) {
      return Option<T>(GetLast());
    }
    for (int i = GetLength() - 1; i >= 0; --i) {
      T value = Get(i);
      if (pred(value)) {
        return Option<T>(value);
      }
    }
    return Option<T>::None();
  }

  template <class U>
  U Reduce(U initial, const std::function<U(const U&, const T&)>& reducer) const {
    U result = initial;
    for (int i = 0; i < GetLength(); ++i) {
      result = reducer(result, Get(i));
    }
    return result;
  }

  Sequence<T>* Where(const std::function<bool(const T&)>& pred) const {
    std::unique_ptr<Sequence<T>> result(CreateEmpty());
    for (int i = 0; i < GetLength(); ++i) {
      T value = Get(i);
      if (pred(value)) {
        Sequence<T>* next = result->Append(value);
        detail::TakeResult(result, next);
      }
    }
    return result.release();
  }

  T operator[](int index) const {
    return Get(index);
  }

  std::unique_ptr<IEnumerator<T>> GetEnumerator() const;
};

template <class T>
class SequenceEnumerator : public IEnumerator<T> {
 public:
  explicit SequenceEnumerator(const Sequence<T>& sequence)
      : sequence_(sequence), index_(-1), current_() {
  }

  bool MoveNext() override {
    if (index_ + 1 >= sequence_.GetLength()) {
      return false;
    }
    ++index_;
    current_ = sequence_.Get(index_);
    return true;
  }

  T Current() const override {
    return current_;
  }

  void Reset() override {
    index_ = -1;
    current_ = T();
  }

 private:
  const Sequence<T>& sequence_;
  int index_;
  T current_;
};

template <class T>
std::unique_ptr<IEnumerator<T>> Sequence<T>::GetEnumerator() const {
  return std::make_unique<SequenceEnumerator<T>>(*this);
}

}
