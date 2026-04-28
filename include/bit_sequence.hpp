#pragma once

#include <cstdint>
#include <string>

#include "bit.hpp"
#include "dynamic_array.hpp"
#include "sequence.hpp"

//unsiged int
//итераторы

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

  Bit GetFirst() const override {
    if (length_ == 0) {
      throw EmptyCollection("Ошибка, длина равна 0");
    }
    return Get(0);
  }

  Bit GetLast() const override {
    if (length_ == 0) {
      throw EmptyCollection("Ошибка, длина равна 0");
    }
    return Get(length_ - 1);
  }

  Bit Get(int index) const override {
    ValidateIndex(index);
    return Bit(GetBitInternal(index));
  }

  int GetLength() const override {
    return length_;
  }

  Sequence<Bit>* GetSubsequence(int start_index, int end_index) const override {
    ValidateClosedRange(start_index, end_index);
    BitSequence* result = new BitSequence();
    result->ResizeBits(end_index - start_index + 1);
    for (int i = start_index; i <= end_index; ++i) {
      result->SetBitInternal(i - start_index, GetBitInternal(i));
    }
    return result;
  }

  Sequence<Bit>* Append(const Bit& item) override {
    ResizeBits(length_ + 1);
    SetBitInternal(length_ - 1, static_cast<bool>(item));
    return this;
  }

  Sequence<Bit>* Prepend(const Bit& item) override {
    return InsertAt(item, 0, true);
  }

  Sequence<Bit>* InsertAt(const Bit& item, int index) override {
    if (index < 0 || index >= length_) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    return InsertAt(item, index, false);
  }

  Sequence<Bit>* Set(int index, const Bit& item) override {
    ValidateIndex(index);
    SetBitInternal(index, static_cast<bool>(item));
    return this;
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

  bool GetBitInternal(int index) const {
    const std::uint64_t word = words_.Get(WordIndex(index));
    return ((word >> BitOffset(index)) & 1ULL) != 0ULL;
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

  void ValidateClosedRange(int start_index, int end_index) const {
    if (length_ == 0) {
      throw EmptyCollection("Ошибка, длина равна 0");
    }
    if (start_index < 0 || end_index < 0 || start_index >= length_ || end_index >= length_) {
      throw IndexOutOfRange("Ошибка, индексы не из диапазона");
    }
    if (start_index > end_index) {
      throw InvalidArgument("Ошибка, начальный индекс больше конечного");
    }
  }

  static int WordIndex(int bit_index) {
    return bit_index / 64;
  }

  Sequence<Bit>* InsertAt(const Bit& item, int index, bool allow) {
    if (!allow && length_ == 0) {
      throw IndexOutOfRange("Ошибка, пустая последовательность");
    }
    if (allow && index == 0 && length_ == 0) {
      ResizeBits(1);
      SetBitInternal(0, static_cast<bool>(item));
      return this;
    }
    if (index < 0 || index > length_ || (!allow && index == length_)) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    const int old_length = length_;
    ResizeBits(length_ + 1);
    for (int i = old_length; i > index; --i) {
      SetBitInternal(i, GetBitInternal(i - 1));
    }
    SetBitInternal(index, static_cast<bool>(item));
    return this;
  }

  }
}
}