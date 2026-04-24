#pragma once

#include <cstdint>

namespace lab2 {

class Bit {
 public:
  Bit() : value_(false) {
  }

  Bit(bool value) : value_(value) {
  }

  Bit(int value) : value_(value != 0) {
  }

  bool Value() const {
    return value_;
  }

  explicit operator bool() const {
    return value_;
  }

  char ToChar() const {
    return value_ ? '1' : '0';
  }

  Bit operator&(const Bit& other) const {
    return Bit(value_ && other.value_);
  }

  Bit operator|(const Bit& other) const {
    return Bit(value_ || other.value_);
  }

  Bit operator^(const Bit& other) const {
    return Bit(value_ != other.value_);
  }

  Bit operator~() const {
    return Bit(!value_);
  }

  bool operator==(const Bit& other) const {
    return value_ == other.value_;
  }

  bool operator!=(const Bit& other) const {
    return !(*this == other);
  }

 private:
  bool value_;
};

}
