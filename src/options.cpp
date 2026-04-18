#include "options.hpp"

namespace lab2 {

template <class T>
Option<T>::Option() : has_value_(false), value_() {
}

template <class T>
Option<T>::Option(const T& value) : has_value_(true), value_(value) {
}

template <class T>
bool Option<T>::HasValue() const {
  return has_value_;
}

template <class T>
Option<T>::operator bool() const {
  return has_value_;
}

template <class T>
const T& Option<T>::Value() const {
  return value_;
}

template <class T>
T& Option<T>::Value() {
  return value_;
}

template <class T>
const T& Option<T>::ValueOr(const T& fallback) const {
  return has_value_ ? value_ : fallback;
}

template <class T>
Option<T> Option<T>::None() {
  return Option<T>();
}

}