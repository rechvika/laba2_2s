#pragma once

#include <cstdint>
#include <string>

#include "bit.hpp"
#include "dinamic_array.hpp"
#include "sequence.hpp"

namespace lab2 {

class BitSequence : public Sequence<Bit> {
 public:
  BitSequence() : words_(), length_(0) {
  }

  explicit BitSequence(const std::string& bits) : words_(), length_(0) {
    for (std::size_t i = 0; i < bits.length(); i++) {
      char ch = bits[i];
      if (ch != '0' && ch != '1') {
        throw InvalidArgument("Ошибка, элемент не удовлетворяет условиям");
      }
      Append(Bit(ch == '1'));
    }
  }

  BitSequence(const Bit* items, size_t count) : words_(), length_(0) {
    ResizeBits(count);
    for (size_t i = 0; i < count; ++i) {
      SetBitInternal(i, items[i].Value() != 0);
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

  Bit Get(size_t index) const override {
    ValidateIndex(index);
    return Bit(GetBitInternal(index));
  }

  size_t GetLength() const override {
    return length_;
  }

  BitSequence& operator=(const BitSequence& other) {
    if (this != &other) {
      words_ = other.words_;
      length_ = other.length_;
    }
    return *this;
  }

  Sequence<Bit>* GetSubsequence(size_t start_index, size_t end_index) const override {
    ValidateClosedRange(start_index, end_index);
    BitSequence* result = new BitSequence();
    result->ResizeBits(end_index - start_index + 1);
    for (size_t i = start_index; i <= end_index; ++i) {
      result->SetBitInternal(i - start_index, GetBitInternal(i));
    }
    return result;
  }

  Sequence<Bit>* Append(const Bit& item) override {
    ResizeBits(length_ + 1);
    SetBitInternal(length_ - 1, item.Value() != 0);
    return this;
  }

  Sequence<Bit>* Prepend(const Bit& item) override {
    return InsertAt(item, 0, true);
  }

  Sequence<Bit>* InsertAt(const Bit& item, size_t index) override {
    if (index >= length_) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    return InsertAt(item, index, false);
  }

  Sequence<Bit>* Set(size_t index, const Bit& item) override {
    ValidateIndex(index);
    SetBitInternal(index, item.Value() != 0);
    return this;
  }

  Sequence<Bit>* Slice(size_t index, size_t count, const Sequence<Bit>* replacement = nullptr) override {

    const size_t start = NormalizeSliceIndex(index);
    const size_t available = length_ - start;
    const size_t remove_count = (count < available) ? count : available;
    BitSequence rebuilt;
    for (size_t i = 0; i < start; ++i) {
      rebuilt.Append(Get(i));
    }
    if (replacement != nullptr) {
      for (size_t i = 0; i < replacement->GetLength(); ++i) {
        rebuilt.Append(replacement->Get(i));
      }
    }
    for (size_t i = start + remove_count; i < length_; ++i) {
      rebuilt.Append(Get(i));
    }
    *this = rebuilt;
    return this;
  }

  Sequence<Bit>* Concat(const Sequence<Bit>& other) override {
    for (size_t i = 0; i < other.GetLength(); ++i) {
      Append(other.Get(i));
    }
    return this;
  }

  Sequence<Bit>* Clone() const override {
    return new BitSequence(*this);
  }

  Sequence<Bit>* CreateEmpty() const override {
    return new BitSequence();
  }

  const char* StorageName() const override {
    return "BitSequence";
  }

  BitSequence* And(const BitSequence& other) const {
    ValidateEqualLength(other);
    BitSequence* result = new BitSequence(*this);
    for (size_t i = 0; i < WordCount(); ++i) {
      result->words_.Set(i, words_.Get(i) & other.words_.Get(i));
    }
    result->ClearTailBits();
    return result;
  }

  BitSequence* Or(const BitSequence& other) const {
    ValidateEqualLength(other);
    BitSequence* result = new BitSequence(*this);
    for (size_t i = 0; i < WordCount(); ++i) {
      result->words_.Set(i, words_.Get(i) | other.words_.Get(i));
    }
    result->ClearTailBits();
    return result;
  }

  BitSequence* Xor(const BitSequence& other) const {
    ValidateEqualLength(other);
    BitSequence* result = new BitSequence(*this);
    for (size_t i = 0; i < WordCount(); ++i) {
      result->words_.Set(i, words_.Get(i) ^ other.words_.Get(i));
    }
    result->ClearTailBits();
    return result;
  }

  BitSequence* Not() const {
    BitSequence* result = new BitSequence(*this);
    for (size_t i = 0; i < WordCount(); ++i) {
      result->words_.Set(i, ~words_.Get(i));
    }
    result->ClearTailBits();
    return result;
  }

  std::string ToBitString() const {
    std::string result;
    result.reserve(length_);
    for (size_t i = 0; i < length_; ++i) {
      result.push_back(GetBitInternal(i) ? '1' : '0');
    }
    return result;
  }

 private:
  DynamicArray<std::uint64_t> words_;
  size_t length_;
  
  size_t WordCount() const {
    return (length_ + 63) / 64;
  }

  static size_t BitOffset(size_t bit_index) {
    return bit_index % 64;
  }

  void ResizeBits(size_t new_length) {

    const size_t old_word_count = WordCount();
    const size_t new_word_count = (new_length + 63) / 64;
    if (new_word_count != old_word_count) {
      words_.Resize(new_word_count);
    }
    length_ = new_length;
    ClearTailBits();
  }

  bool GetBitInternal(size_t index) const {
    const std::uint64_t word = words_.Get(WordIndex(index));
    return ((word >> BitOffset(index)) & 1ULL) != 0ULL;
  }

  void SetBitInternal(size_t index, bool value) {
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
    const size_t used_bits = length_ % 64;
    if (used_bits == 0) {
      return;
    }
    const std::uint64_t mask = (1ULL << used_bits) - 1ULL;
    words_.Set(WordCount() - 1, words_.Get(WordCount() - 1) & mask);
  }

  void ValidateIndex(size_t index) const {
    if (index >= length_) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
  }

  void ValidateClosedRange(size_t start_index, size_t end_index) const {
    if (length_ == 0) {
      throw EmptyCollection("Ошибка, длина равна 0");
    }
    if (start_index >= length_ || end_index >= length_) {
      throw IndexOutOfRange("Ошибка, индексы не из диапазона");
    }
    if (start_index > end_index) {
      throw InvalidArgument("Ошибка, начальный индекс больше конечного");
    }
  }

  size_t NormalizeSliceIndex(size_t index) const {
    size_t normalized = index;
    if (normalized > length_) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    return normalized;
  }

  void ValidateEqualLength(const BitSequence& other) const {
    if (length_ != other.length_) {
      throw InvalidArgument("Ошибка, длины не равны");
    }
  }

  static size_t WordIndex(size_t bit_index) {
    return bit_index / 64;
  }

  Sequence<Bit>* InsertAt(const Bit& item, size_t index, bool allow) {
    if (!allow && length_ == 0) {
      throw IndexOutOfRange("Ошибка, пустая последовательность");
    }
    if (allow && index == 0 && length_ == 0) {
      ResizeBits(1);
      SetBitInternal(0, item.Value() != 0);
      return this;
    }
    if (index > length_ || (!allow && index == length_)) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    const size_t old_length = length_;
    ResizeBits(length_ + 1);
    for (size_t i = old_length; i > index; --i) {
      SetBitInternal(i, GetBitInternal(i - 1));
    }
    SetBitInternal(index, item.Value() != 0);
    return this;
  }
};

}