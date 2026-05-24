#pragma once

#include <utility>
#include <cstddef>
#include <new>
#include "exception.hpp"

namespace lab2 {

template <class T>
class Option {
public:
    Option() noexcept : has_value_(false) {}
    
    Option(const T& value) : has_value_(true) {
        new (storage_) T(value);
    }
    
    Option(T&& value) : has_value_(true) {
        new (storage_) T(std::move(value));
    }
    
    ~Option() {
        if (has_value_) {
            reinterpret_cast<T*>(storage_)->~T();
        }
    }
    
    Option(const Option& other) : has_value_(other.has_value_) {
        if (has_value_) {
            new (storage_) T(*reinterpret_cast<const T*>(other.storage_));
        }
    }
    
    Option(Option&& other) noexcept : has_value_(other.has_value_) {
        if (has_value_) {
            new (storage_) T(std::move(*reinterpret_cast<T*>(other.storage_)));
            other.has_value_ = false;
        }
    }
    
    Option& operator=(const Option& other) {
        if (this != &other) {
            if (has_value_ && other.has_value_) {
                **reinterpret_cast<T*>(storage_) = *reinterpret_cast<const T*>(other.storage_);
            } else if (other.has_value_) {
                new (storage_) T(*reinterpret_cast<const T*>(other.storage_));
                has_value_ = true;
            } else if (has_value_) {
                reinterpret_cast<T*>(storage_)->~T();
                has_value_ = false;
            }
        }
        return *this;
    }
    
    Option& operator=(Option&& other) noexcept {
        if (this != &other) {
            if (has_value_ && other.has_value_) {
                **reinterpret_cast<T*>(storage_) = std::move(*reinterpret_cast<T*>(other.storage_));
                other.has_value_ = false;
            } else if (other.has_value_) {
                new (storage_) T(std::move(*reinterpret_cast<T*>(other.storage_)));
                has_value_ = true;
                other.has_value_ = false;
            } else if (has_value_) {
                reinterpret_cast<T*>(storage_)->~T();
                has_value_ = false;
            }
        }
        return *this;
    }
    
    bool HasValue() const noexcept {
        return has_value_;
    }
    
    explicit operator bool() const noexcept {
        return has_value_;
    }
    
    T& Value() {
        if (!has_value_) {
            throw InvalidArgument("Ошибка, Option не имеет значения");
        }
        return *reinterpret_cast<T*>(storage_);
    }
    
    const T& Value() const {
        if (!has_value_) {
            throw InvalidArgument("Ошибка, Option не имеет значения");
        }
        return *reinterpret_cast<const T*>(storage_);
    }
    
    const T& ValueOr(const T& fallback) const {
        return has_value_ ? *reinterpret_cast<const T*>(storage_) : fallback;
    }
    
    T ValueOr(T&& fallback) const {
        return has_value_ ? *reinterpret_cast<const T*>(storage_) : std::move(fallback);
    }
    
    static Option<T> None() noexcept {
        return Option<T>();
    }
    
private:
    bool has_value_;
    alignas(T) std::byte storage_[sizeof(T)];
};

}