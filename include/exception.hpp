#pragma once

#include <stdexcept>
#include <string>

namespace lab2 {

class LabException : public std::runtime_error {
 public:
  explicit LabException(const std::string& message) : std::runtime_error(message) {
  }
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