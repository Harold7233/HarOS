//
// Created by Harold on 2020/6/1.
//

#ifndef OS_CPP_PTYMASTER_HPP
#define OS_CPP_PTYMASTER_HPP

#include <devices/CharacterDevice.hpp>
#include <devices/Keyboard.hpp>

namespace kernel {

    class PTYMaster : public CharacterDevice {
    public:
        virtual uint8_t read() override;

        virtual size_t write(uint8_t) override;

        virtual size_t read(uint8_t *, size_t) override;

        virtual size_t write(uint8_t *, size_t) override;

        void receive_key(KeyCombination);
    };
}


#endif //OS_CPP_PTYMASTER_HPP
