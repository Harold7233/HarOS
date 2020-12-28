//
// Created by Harold on 2020/5/26.
//

#ifndef OS_CPP_AUTOLOCKER_HPP
#define OS_CPP_AUTOLOCKER_HPP

#include <sys/types.hpp>

namespace kernel {
    template<typename Locker>
    class AutoLocker {
    private:
        Locker &locker_;
    public:
        AutoLocker(Locker &locker) : locker_(locker) {
            locker_.acquire();
        }

        ~AutoLocker() {
            locker_.release();
        }
    };
}
#endif //OS_CPP_AUTOLOCKER_HPP
