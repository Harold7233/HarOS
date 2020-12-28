//
// Created by Harold on 2020/6/7.
//

#ifndef OS_CPP_SINGLETON_HPP
#define OS_CPP_SINGLETON_HPP


namespace kernel {
    template<typename T>
    class Singleton {
    protected:
        static T *ins_;
    public:
        inline static T &instance() {
            return ins_;
        }
    };
}

#endif //OS_CPP_SINGLETON_HPP
