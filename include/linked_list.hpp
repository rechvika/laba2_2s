#pragma once

#include <cstddef>
#include <utility>

#include "lab2/exceptions.hpp"
#include "lab2/icollection.hpp"

namespace lab2 {

template <class T>
class LinkedList : public ICollection<T> {
 private:
  struct Node {
    T value;
    Node* next;

    explicit Node(const T& value) : value(value), next(nullptr) {
    }
  };

 public:
  LinkedList() : head_(nullptr), tail_(nullptr), length_(0) {
  }

  LinkedList(const T* items, int count) : LinkedList() {
    if (count < 0) {
      throw InvalidArgument("LinkedList: negative count");
    }
    for (int i = 0; i < count; ++i) {
      Append(items[i]);
    }
  }

  LinkedList(const LinkedList<T>& other) : LinkedList() {
    Node* node = other.head_;
    while (node != nullptr) {
      Append(node->value);
      node = node->next;
    }
  }

  LinkedList<T>& operator=(const LinkedList<T>& other) {
    if (this == &other) {
      return *this;
    }
    LinkedList<T> copy(other);
    Swap(copy);
    return *this;
  }

  ~LinkedList() override {
    Clear();
  }

  T GetFirst() const {
    if (length_ == 0) {
      throw EmptyCollection("Ошибка, список пуст");
    }
    return head_->value;
  }

  T GetLast() const {
    if (length_ == 0) {
      throw EmptyCollection("Ошибка, список пуст");
    }
    return tail_->value;
  }

  T Get(int index) const {
    ValidateIndex(index);
    return NodeAt(index)->value;
  }

  T Get(std::size_t index) const override {
    if (index > static_cast<std::size_t>(static_cast<int>(index))) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    return Get(static_cast<int>(index));
  }

  LinkedList<T>* GetSubList(int start_index, int end_index) const {
    ValidateRange(start_index, end_index);
    LinkedList<T>* result = new LinkedList<T>();
    for (int i = start_index; i <= end_index; ++i) {
      result->Append(Get(i));
    }
    return result;
  }

  int GetLength() const {
    return length_;
  }

  std::size_t GetCount() const override {
    return static_cast<std::size_t>(length_);
  }

  void Append(const T& item) {
    Node* node = new Node(item);
    if (tail_ == nullptr) {
      head_ = node;
      tail_ = node;
    } else {
      tail_->next = node;
      tail_ = node;
    }
    ++length_;
  }

  void Prepend(const T& item) {
    Node* node = new Node(item);
    node->next = head_;
    head_ = node;
    if (tail_ == nullptr) {
      tail_ = node;
    }
    ++length_;
  }

  void InsertAt(const T& item, int index) {
    if (index < 0 || index >= length_) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
    if (index == 0) {
      Prepend(item);
      return;
    }
    Node* prev = NodeAt(index - 1);
    Node* node = new Node(item);
    node->next = prev->next;
    prev->next = node;
    if (node->next == nullptr) {
      tail_ = node;
    }
    ++length_;
  }

  LinkedList<T>* Concat(const LinkedList<T>* other) const {
    if (other == nullptr) {
      throw InvalidArgument("Ошибка, пустой лист");
    }
    LinkedList<T>* result = new LinkedList<T>(*this);
    Node* node = other->head_;
    while (node != nullptr) {
      result->Append(node->value);
      node = node->next;
    }
    return result;
  }

 private:
  Node* head_;
  Node* tail_;
  int length_;

  Node* NodeAt(int index) const {
    Node* node = head_;
    for (int i = 0; i < index; ++i) {
      node = node->next;
    }
    return node;
  }

  void ValidateIndex(int index) const {
    if (index < 0 || index >= length_) {
      throw IndexOutOfRange("Ошибка, индекс не из диапазона");
    }
  }

  void ValidateRange(int start_index, int end_index) const {
    ValidateIndex(start_index);
    ValidateIndex(end_index);
    if (start_index > end_index) {
      throw InvalidArgument("Ошибка, недоступный диапазон");
    }
  }

  void Clear() {
    while (head_ != nullptr) {
      Node* next = head_->next;
      delete head_;
      head_ = next;
    }
    tail_ = nullptr;
    length_ = 0;
  }

  void Swap(LinkedList<T>& other) noexcept {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(length_, other.length_);
  }
};

}