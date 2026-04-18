#pragma once

#include <utility>

namespace lab2 {

template <class T>
class Option {
 public:
  Option();
  Option(const T& value);
  
  bool HasValue() const;
  explicit operator bool() const;
  const T& Value() const;
  T& Value();
  const T& ValueOr(const T& fallback) const;
  static Option<T> None();

 private:
  bool has_value_;
  T value_;
};

}