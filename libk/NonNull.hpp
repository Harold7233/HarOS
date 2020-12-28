//
// Created by Harold on 2020/5/21.
//

#ifndef OS_CPP_NONNULL_HPP
#define OS_CPP_NONNULL_HPP

#include <libcpp/assert.hpp>

template<typename T>
class NonNull {
private:
    T *ptr{nullptr};

    inline void validate() {
        assert(ptr != nullptr);
    }

public:
    NonNull() = default;

    explicit NonNull(T *ptr) : ptr(ptr) {
        assert(ptr != nullptr);
    }

    NonNull(const NonNull &rhs) {
        validate();
        ptr = rhs.ptr;
    }

    NonNull &operator++() {
        validate();
        ++ptr;
        return *this;
    }

//    const NonNull operator++(int) {
//        validate();
//        return *this;
//    }
    NonNull &operator--() {
        validate();
        --ptr;
        return *this;
    }
//    NonNull &operator--(int){
//        validate();
//        --ptr;
//        return *this;
//    }


    T &operator*() {
        validate();
        return *ptr;
    }

    T *&operator->() {
        validate();
        return ptr;
    }

    bool operator==(T *rhs) {
        return ptr == rhs;
    }

    bool operator==(NonNull &rhs) {
        return ptr == rhs.ptr;
    }

    bool operator!=(T *rhs) {
        return ptr != rhs;
    }

    bool operator!=(NonNull &rhs) {
        return ptr != rhs.ptr;
    }

    explicit operator bool() {
        return bool(ptr);
    }

    NonNull &operator=(const NonNull &rhs) {
        if (this == &rhs) {
            return *this;
        }
        ptr = rhs.ptr;
        return *this;
    }

    NonNull &operator=(const T *rhs) {
        ptr = const_cast<T *>(rhs);
        return *this;
    }

    // 隐式转换
    // NonNull<T> a;
    // T *b = a;
    operator T *() {
        return ptr;
    }

};

#endif //OS_CPP_NONNULL_HPP
