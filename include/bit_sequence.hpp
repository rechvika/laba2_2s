#pragma once

#include <cstdint>
#include <string>

#include "bit.hpp"
#include "dynamic_array.hpp"
#include "sequence.hpp"

namespace lab2 {

class BitSequence : public Sequence<Bit> {
 public:
  BitSequence() : words_(), length_(0) {
  }

  explicit BitSequence(const std::string& bits) : words_(), length_(0) {
    for (int i = 0; i < bits.length(); i++) {
      char ch = bits[i]; {
      if (ch != '0' && ch != '1') {
        throw InvalidArgument("Ошибка, элемент не удовлетворяет условиям");
      }
      Append(Bit(ch == '1'));
    }
  }

  BitSequence(const Bit* items, int count) : words_(), length_(0) {
    if (count < 0) {
      throw InvalidArgument("Ошибка, отрицательное количество битов");
    }
    ResizeBits(count);
    for (int i = 0; i < count; ++i) {
      SetBitInternal(i, static_cast<bool>(items[i]));
    }
  }

  BitSequence(const BitSequence& other) : words_(other.words_), length_(other.length_) {
  }

  Sequence<Bit>* Set(int index, const Bit& item) override {
    ValidateIndex(index);
    SetBitInternal(index, static_cast<bool>(item));
    return this;
  }

  Bit Get(int index) const override {
    ValidateIndex(index);
    return Bit(GetBitInternal(index));
  }

  void ResizeBits(int new_length) {
    if (new_length < 0) {
      throw InvalidArgument("Ошибка, отрицательная длина");
    }
    const int old_word_count = WordCount();
    const int new_word_count = (new_length + 63) / 64;
    if (new_word_count != old_word_count) {
      words_.Resize(new_word_count);
    }
    length_ = new_length;
    ClearTailBits();
  }

  void SetBitInternal(int index, bool value) {
    std::uint64_t word = words_.Get(WordIndex(index));
    const std::uint64_t mask = 1ULL << BitOffset(index);
    if (value) {
      word |= mask;
    } else {
      word &= ~mask;
    }
    words_.Set(WordIndex(index), word);
  }

 private:
  DynamicArray<std::uint64_t> words_;
  int length_;
  
  int WordCount() const {
    return (length_ + 63) / 64;
  }

  static int BitOffset(int bit_index) {
    return bit_index % 64;
  }

  void ClearTailBits() {
    if (length_ == 0 || WordCount() == 0) {
      return;
    }
    const int used_bits = length_ % 64;
    if (used_bits == 0) {
      return;
    }
    const std::uint64_t mask = (1ULL << used_bits) - 1ULL;
    words_.Set(WordCount() - 1, words_.Get(WordCount() - 1) & mask);
  }

  void ValidateIndex(int index) const {
    if (index < 0 || index >= length_) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
  }

  static int WordIndex(int bit_index) {
    return bit_index / 64;
  }

  }
}
}