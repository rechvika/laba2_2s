#pragma once

#include <cmath>
#include <functional>
#include <limits>
#include <tuple>
#include <utility>

#include "array_sequence.hpp"
#include "dinamic_array.hpp"

namespace lab2 {

template <class T, class U>
Sequence<U>* Map(const Sequence<T>& sequence, const std::function<U(const T&)>& mapper) {
    std::unique_ptr<Sequence<U>> result(new MutableArraySequence<U>());
    auto enumerator = sequence.GetEnumerator();
    while (enumerator->MoveNext()) {
        Sequence<U>* next = result->Append(mapper(enumerator->Current()));
        detail::TakeResult(result, next);
    }
    return result.release();
}

template <class T, class U>
Sequence<U>* MapIndexed(
    const Sequence<T>& sequence, 
    const std::function<U(const T&, size_t)>& mapper
) {
    std::unique_ptr<Sequence<U>> result(new MutableArraySequence<U>());
    auto enumerator = sequence.GetEnumerator();
    size_t index = 0;
    while (enumerator->MoveNext()) {
        Sequence<U>* next = result->Append(mapper(enumerator->Current(), index));
        detail::TakeResult(result, next);
        ++index;
    }
    return result.release();
}

template <class T, class U>
Sequence<U>* FlatMap(const Sequence<T>& sequence, const std::function<DynamicArray<U>(const T&)>& mapper) {
    std::unique_ptr<Sequence<U>> result(new MutableArraySequence<U>());
    auto enumerator = sequence.GetEnumerator();
    while (enumerator->MoveNext()) {
        const DynamicArray<U> values = mapper(enumerator->Current());
        auto valuesEnum = values.GetEnumerator();
        while (valuesEnum->MoveNext()) {
            Sequence<U>* next = result->Append(valuesEnum->Current());
            detail::TakeResult(result, next);
        }
    }
    return result.release();
}

template <class T1, class T2>
Sequence<std::pair<T1, T2>>* Zip(const Sequence<T1>& left, const Sequence<T2>& right) {
  std::unique_ptr<Sequence<std::pair<T1, T2>>> result(new MutableArraySequence<std::pair<T1, T2>>());
    auto leftEnum = left.GetEnumerator();
    auto rightEnum = right.GetEnumerator();
    while (leftEnum->MoveNext() && rightEnum->MoveNext()) {
        auto* next = result->Append(std::make_pair(leftEnum->Current(), rightEnum->Current()));
        detail::TakeResult(result, next);
    }
    return result.release();
}

template <class T1, class T2>
std::pair<Sequence<T1>*, Sequence<T2>*> Unzip(const Sequence<std::pair<T1, T2>>& sequence) {
  std::unique_ptr<Sequence<T1>> first(new MutableArraySequence<T1>());
  std::unique_ptr<Sequence<T2>> second(new MutableArraySequence<T2>());
  auto enumerator = sequence.GetEnumerator();
    while (enumerator->MoveNext()) {
        auto value = enumerator->Current();
        detail::TakeResult(first, first->Append(value.first));
        detail::TakeResult(second, second->Append(value.second));
  }
    return std::make_pair(first.release(), second.release());
  }

template <class T>
Sequence<T>* Range(T start, T stop, T step = static_cast<T>(1)) {
  if (step == static_cast<T>(0)) {
    throw InvalidArgument("Ошибка, шаг равен 0");
  }
  std::unique_ptr<Sequence<T>> result(new MutableArraySequence<T>());
  if (step > static_cast<T>(0)) {
    for (T value = start; value < stop; value = static_cast<T>(value + step)) {
      detail::TakeResult(result, result->Append(value));
    }
  } else {
    for (T value = start; value > stop; value = static_cast<T>(value + step)) {
      detail::TakeResult(result, result->Append(value));
    }
  }
  return result.release();
}

}