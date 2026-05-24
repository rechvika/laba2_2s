#pragma once

#include <string>

namespace lab2 {

class LabException {
 public:
  explicit LabException(const std::string& message) : message_(message) {
  }

  virtual ~LabException() = default;

  virtual const char* what() const noexcept {
    return message_.c_str();
  }

 private:
  std::string message_;
};

class InvalidArgument : public LabException {
 public:
  explicit InvalidArgument(const std::string& message) : LabException(message) {
  }
};

class IndexOutOfRange : public LabException {
 public:
  explicit IndexOutOfRange(const std::string& message) : LabException(message) {
  }
};

class EmptyCollection : public LabException {
 public:
  explicit EmptyCollection(const std::string& message) : LabException(message) {
  }
};

}