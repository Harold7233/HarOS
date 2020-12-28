//
// Created by Harold on 2020/5/3.
//
#include <libcpp/String.hpp>
#include <libcpp/assert.hpp>


String::String() : length(0), c_string_("") {
}

String::String(const char *c_str) : length(size_of_c_str(c_str)), c_string_(c_str) {
    assert(c_str != nullptr);
}

String String::operator+(String &rhs) const {
    String new_string;
    new_string.c_string_ = new char[size() + rhs.size() + 1];
    char *dst = const_cast<char *>(new_string.c_string_);
    memcpy(dst, c_string_, size());
    memcpy(dst + size(), rhs.c_string_, rhs.size());
    dst[size() + rhs.size()] = 0;
    return new_string;
}

String::String(String &&other) noexcept {
    c_string_ = other.c_string_;
    if (other.reallocated) {
        reallocated = true;
        other.reallocated = false;
        other.c_string_ = nullptr;
    }


}

String &String::operator=(String &rhs) {
    if (&rhs == this){
        return *this;
    }
//
}




