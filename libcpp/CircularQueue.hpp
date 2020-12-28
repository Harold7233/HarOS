//
// Created by Harold on 2020/6/17.
//

#ifndef OS_LIB_TEST_CIRCULARQUEUE_HPP
#define OS_LIB_TEST_CIRCULARQUEUE_HPP

#include <sys/types.hpp>
#include <kernel/memory/new.hpp>

template<typename T>
class CircularQueue {
private:
    uint8_t *data_;
    size_t head_{0};
    size_t tail_{0};
    size_t capacity_;
    size_t size_{0};

    inline void set_element_at(size_t index, const T &element) {
        T *dst = reinterpret_cast<T *>(data_ + sizeof(T) * index);
        *dst = element;
    }

public:
    CircularQueue(size_t capacity) {
        capacity_ = capacity;
        data_ = new(alignof(T))uint8_t[capacity * sizeof(T)];
    }

    bool enqueue(const T &element) {
        if (size_ == capacity_) {
            return false;
        }
        if (size_ == 0) {
            head_ = tail_ = 0;
        } else {
            tail_ = (tail_ + 1) % capacity_;
        }
        set_element_at(tail_, element);
        ++size_;
        return true;
    }

    bool enqueue(const T *elements, size_t count) {
        if (size_ + count > capacity_) {
            return false;
        }
        int i = 0;
        if (size_ == 0) {
            head_ = tail_ = 0;
            set_element_at(tail_, elements[i++]);
            --count;
        }
        while (count--) {
            tail_ = (tail_ + 1) % capacity_;
            set_element_at(tail_, elements[i++]);
        }
    }

    inline bool empty() {
        return size_ == 0;
    }

    T dequeue() {
        if (size_ == 0) {
            // TODO panic
        }
        size_t dst = head_;
        head_ = (head_ + 1) % capacity_;
        --size_;
        return *reinterpret_cast<T *>(data_ + sizeof(T) * dst);
    }
};


#endif //OS_LIB_TEST_CIRCULARQUEUE_HPP
