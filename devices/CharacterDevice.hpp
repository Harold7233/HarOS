//
// Created by Harold on 2020/5/12.
//

#ifndef OS_CPP_CHARACTERDEVICE_HPP
#define OS_CPP_CHARACTERDEVICE_HPP

#include <devices/Device.hpp>

namespace kernel {
    class CharacterDevice : Device {
        virtual uint8_t read() = 0;

        virtual size_t write(uint8_t) = 0;

        virtual size_t read(uint8_t *, size_t) = 0;

        virtual size_t write(uint8_t *, size_t) = 0;
    };
}

#endif //OS_CPP_CHARACTERDEVICE_HPP
