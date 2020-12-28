//
// Created by Harold on 2020/5/3.
//

#ifndef OS_CPP_STRING_HPP
#define OS_CPP_STRING_HPP

#include <sys/types.hpp>
#include <libcpp/cstring.hpp>

class String {
private:
    size_t length{};
    bool reallocated{false};
    const char *c_string_{};

    size_t size_of_c_str(const char *c_str) const {
        size_t s = 0;
        while (*c_str++) {
            ++s;
        }
        return s;
    }

public:
    String();

    String(const char *);

    size_t size() const {
        return length;
    }

    String operator+(String &rhs) const;

    String &operator=(String &rhs);

    String(String &&other) noexcept;

    ~String() {
        if (reallocated)
            delete[] c_string_;
    }
};


#endif //OS_CPP_STRING_HPP
