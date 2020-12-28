//
// Created by Harold on 2020/6/17.
//

#ifndef OS_CPP_NEW_HPP
#define OS_CPP_NEW_HPP

#include <sys/types.hpp>

void *operator new(size_t size) noexcept;

void operator delete(void *addr) noexcept;

void *operator new(size_t size, align_val_t alignment);

void *operator new(size_t size, void *ptr) noexcept;

void *operator new[](size_t size) noexcept;

void *operator new[](size_t size, align_val_t alignment);

void operator delete[](void *addr) noexcept;

#endif //OS_CPP_NEW_HPP
