#pragma once

#include <utility>

namespace lab2 {

template <class T>
class Option {
 public:
  Option() : has_value_(false), value_() {
  }

  Option(const T& value) : has_value_(true), value_(value) {
  }

  bool HasValue() const {
    return has_value_;
  }

  explicit operator bool() const {
    return has_value_;
  }

  const T& Value() const {
    return value_;
  }

  T& Value() {
    return value_;
  }

  const T& ValueOr(const T& fallback) const {
    return has_value_ ? value_ : fallback;
  }

  static Option<T> None() {
    return Option<T>();
  }

 private:
  bool has_value_;
  T value_;
};

}
