//
// Created by Harold on 2020/5/15.
//

#ifndef OS_CPP_CALLABLE_HPP
#define OS_CPP_CALLABLE_HPP

#include <sys/types.hpp>

class F {
};

template<typename T>
class Callable {
    const T *obj;
public:
    explicit Callable(const T *f) : obj(obj) {
    }

    Callable() = delete;

    // TODO decltype引用？
    decltype(T()) operator()() {
        return T();
    }

};

#endif //OS_CPP_CALLABLE_HPP
