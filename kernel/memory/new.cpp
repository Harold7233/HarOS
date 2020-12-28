//
// Created by Harold on 2020/6/17.
//

#include <kernel/memory/new.hpp>
#include <kernel/memory/MemoryManager.hpp>

void *operator new(size_t size) noexcept {
    return MemoryManager::kmalloc(size);
}

void operator delete(void *addr) noexcept {
    MemoryManager::kfree(addr);
}

void *operator new(size_t size, align_val_t alignment) {
    return MemoryManager::kmalloc_aligned(size, alignment);
}

void *operator new(size_t size, void *ptr) noexcept {
    return ptr;
}

void *operator new[](size_t size) noexcept {
    return MemoryManager::kmalloc(size);
}

void *operator new[](size_t size, align_val_t alignment) {
    return MemoryManager::kmalloc_aligned(size, alignment);
}

void operator delete[](void *addr) noexcept {
    MemoryManager::kfree(addr);
}

extern "C" void UNUSED __gxx_personality_v0() {}
extern "C" void UNUSED _Unwind_Resume() {}
extern "C" size_t UNUSED __cxa_throw_bad_array_new_length() {}