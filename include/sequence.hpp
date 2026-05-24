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
  auto id(T t) -> T {
    return t;
  }

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
  virtual T Get(size_t index) const = 0;
  virtual size_t GetLength() const = 0;
  std::size_t GetCount() const override {
    return static_cast<std::size_t>(GetLength());
  }

  virtual Sequence<T>* GetSubsequence(size_t start_index, size_t end_index) const = 0;
  virtual Sequence<T>* Append(const T& item) = 0;
  virtual Sequence<T>* Prepend(const T& item) = 0;
  virtual Sequence<T>* InsertAt(const T& item, size_t index) = 0;
  virtual Sequence<T>* Concat(const Sequence<T>& other) = 0;

  virtual Sequence<T>* Set(size_t index, const T& item) = 0;
  virtual Sequence<T>* Slice(size_t index, size_t count, const Sequence<T>* replacement = nullptr) = 0;

  virtual Sequence<T>* Clone() const = 0;
  virtual Sequence<T>* CreateEmpty() const = 0;
  virtual const char* StorageName() const = 0;

  Option<T> TryFirst() const {
    if (GetLength() == 0) {
        return Option<T>::None();
    }
    return Option<T>(GetFirst());
  }

  Option<T> TryFirst(const std::function<bool(const T&)>& pred) const {
    if (!pred) {
        throw InvalidArgument("Передана пустая предикатная функция");
    }
    
    if (GetLength() == 0) {
        return Option<T>::None();
    }

    auto enumerator = GetEnumerator();
    while (enumerator->MoveNext()) {
        T value = enumerator->Current();
        if (pred(value)) {
            return Option<T>(value);
        }
    }
    return Option<T>::None();
  }

  Option<T> TryLast() const {
    if (GetLength() == 0) {
        return Option<T>::None();
    }
    return Option<T>(GetLast());
  }

  Option<T> TryLast(const std::function<bool(const T&)>& pred) const {
    if (!pred) {
        throw InvalidArgument("Передана пустая предикатная функция");
    }
    
    if (GetLength() == 0) {
        return Option<T>::None();
    }

    for (size_t i = GetLength(); i > 0; --i) {
        const T& value = Get(i - 1);
        if (pred(value)) {
            return Option<T>(value);
        }
    }
    return Option<T>::None();
}

  template <class U>
  U Reduce(U initial, const std::function<U(const U&, const T&)>& reducer) const {
    U result = initial;
    auto enumerator = GetEnumerator();
    while (enumerator->MoveNext()) {
        result = reducer(result, enumerator->Current());
    }
    return result;
}

  Sequence<T>* Where(const std::function<bool(const T&)>& pred) const {
    std::unique_ptr<Sequence<T>> result(CreateEmpty());
    auto enumerator = GetEnumerator();
    while (enumerator->MoveNext()) {
        T value = enumerator->Current();
      if (pred(value)) {
        Sequence<T>* next = result->Append(value);
        detail::TakeResult(result, next);
      }
    }
    return result.release();
  }

  T& operator[](size_t index) {
    throw InvalidArgument("Оператор [] не подходит для неизменяемой последовательности");
  }

  const T& operator[](size_t index) const {
    return Get(index);
  }

  std::unique_ptr<IEnumerator<T>> GetEnumerator() const;
};

template <class T>
class SequenceEnumerator : public IEnumerator<T> {
 public:
  explicit SequenceEnumerator(const Sequence<T>& sequence)
      : sequence_(sequence), index_(0), current_() {
  }

  bool MoveNext() override {
    if (index_ >= sequence_.GetLength()) {
        return false;
    }
    current_ = sequence_.Get(index_);
    ++index_;
    return true;
}
  T Current() const override {
    return current_;
  }

  void Reset() override {
    index_ = 0;
    current_ = T();
  }

 private:
  const Sequence<T>& sequence_;
  size_t index_;
  T current_;
};

template <class T>
std::unique_ptr<IEnumerator<T>> Sequence<T>::GetEnumerator() const {
  return std::make_unique<SequenceEnumerator<T>>(*this);
}

}
