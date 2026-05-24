#pragma once

#include <cstddef>
#include <utility>
#include <memory>

#include "exception.hpp"
#include "icollection.hpp"
#include "ienumerator.hpp"

namespace lab2 {

template <class T>
class LinkedList : public ICollection<T> {
  static constexpr size_t max_size_ = 1024 * 1024 * 100;
 private:
  struct Node {
    T value;
    std::unique_ptr<Node> next;

    explicit Node(const T& value) : value(value), next(nullptr) {
    }
  };

 public:
  LinkedList() : head_(nullptr), tail_(nullptr), length_(0) {
  }

  LinkedList(const T* items, size_t count) : LinkedList() {
    if (count == 0) {
      throw InvalidArgument("Ошибка, попытка создать список из 0 элементов");
    }
    if (items == nullptr) {
      throw InvalidArgument("Ошибка, указатель на массив нулевой");
    }
    if (count > max_size_) {
      throw InvalidArgument("Ошибка, размер " + std::to_string(count) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(max_size_));
    }
    for (size_t i = 0; i < count; ++i) {
      Append(items[i]);
    }
  }

  LinkedList(const LinkedList<T>& other) : LinkedList() {
    auto enumerator = other.GetEnumerator();
    while (enumerator->MoveNext()) {
        Append(enumerator->Current());
    }
  }

  LinkedList<T>& operator=(const LinkedList<T>& other) {
    if (this != &other) {
        Clear();
        auto enumerator = other.GetEnumerator();
        while (enumerator->MoveNext()) {
            Append(enumerator->Current());
        }
    }
    return *this;
  }

  ~LinkedList() override = default;

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

  T Get(size_t index) const {
    ValidateIndex(index);
    return NodeAt(index)->value;
  }
  
  void Set(size_t index, const T& value) {
    ValidateIndex(index);
    NodeAt(index)->value = value;
  }

  LinkedList<T>* GetSubList(size_t start_index, size_t end_index) const {
    ValidateRange(start_index, end_index);
    LinkedList<T>* result = new LinkedList<T>();
    for (size_t i = start_index; i <= end_index; ++i) {
      result->Append(Get(i));
    }
    return result;
  }

  size_t GetLength() const {
    return length_;
  }

  std::size_t GetCount() const override {
    return static_cast<size_t>(length_);
  }

  void Append(const T& item) {
    std::unique_ptr<Node> node = std::make_unique<Node>(item);
    Node* raw_node = node.get();
    if (tail_ == nullptr) {
      head_ = std::move(node);
      tail_ = raw_node;
    } else {
      tail_->next = std::move(node);
      tail_ = raw_node;
    }
    ++length_;
  }

  void Prepend(const T& item) {
    std::unique_ptr<Node> node = std::make_unique<Node>(item);
    node->next = std::move(head_);
    head_ = std::move(node);
    if (tail_ == nullptr) {
      tail_ = head_.get();
    }
    ++length_;
  }

  void InsertAt(const T& item, size_t index) {
    if (index > length_) {
        throw IndexOutOfRange("Ошибка, индекс " + std::to_string(index) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(length_));
    }
    if (index == 0) {
        Prepend(item);
        return;
    }
    if (index == length_) {
        Append(item);
        return;
    }
    Node* prev = NodeAt(index - 1);
    std::unique_ptr<Node> node = std::make_unique<Node>(item);
    node->next = std::move(prev->next);
    prev->next = std::move(node);
    ++length_;
  }

  LinkedList<T>* Concat(const LinkedList<T>* other) const {
    if (other == nullptr) {
      throw InvalidArgument("Ошибка, пустой лист");
    }
    LinkedList<T>* result = new LinkedList<T>(*this);
    Node* node = other->head_.get();
    while (node != nullptr) {
      result->Append(node->value);
      node = node->next.get();
    }
    return result;
  }

  class LinkedListEnumerator : public IEnumerator<T> {
  public:
    explicit LinkedListEnumerator(const LinkedList<T>& list) 
        : list_(list), current_node_(nullptr), index_(0) {}
    
    bool MoveNext() override {
      if (index_ == 0) {
        current_node_ = list_.head_.get();
      } else if (current_node_) {
        current_node_ = current_node_->next.get();
      }
      ++index_;
      return current_node_ != nullptr && index_ <= list_.length_;
    }
    
    T Current() const override {
      if (!current_node_) {
        throw InvalidArgument("Ошибка, нет текущего элемента в итераторе");
      }
      return current_node_->value;
    }
    
    void Reset() override {
      current_node_ = nullptr;
      index_ = 0;
    }
    
  private:
    const LinkedList<T>& list_;
    typename LinkedList<T>::Node* current_node_;
    size_t index_;
  };
  
  std::unique_ptr<IEnumerator<T>> GetEnumerator() const {
    return std::make_unique<LinkedListEnumerator>(*this);
  }

 private:
  std::unique_ptr<Node> head_;
  Node* tail_;
  size_t length_;

  Node* NodeAt(size_t index) const {
    Node* node = head_.get();
    for (size_t i = 0; i < index; ++i) {
      node = node->next.get();
    }
    return node;
  }

  void ValidateIndex(size_t index) const {
    if (index >= length_) {
      throw IndexOutOfRange("Ошибка, индекс " + std::to_string(index) + 
                              " больше допустимого максимального значения " + 
                              std::to_string(length_ - 1));
    }
  }

  void ValidateRange(size_t start_index, size_t end_index) const {
    ValidateIndex(start_index);
    ValidateIndex(end_index);
    if (start_index > end_index) {
      throw InvalidArgument("Ошибка, стартовый индекс " + std::to_string(start_index) + 
                              " больше конечного индекса " + std::to_string(end_index));
    }
  }

  void Clear() {
    head_.reset();
    tail_ = nullptr;
    length_ = 0;
  }

  void Swap(LinkedList<T>& other) noexcept {
    head_.swap(other.head_);
    std::swap(tail_, other.tail_);
    std::swap(length_, other.length_);
  }
};

}