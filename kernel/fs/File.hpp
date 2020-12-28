//
// Created by Harold on 2020/5/17.
//

#ifndef OS_CPP_FILE_HPP
#define OS_CPP_FILE_HPP

#include <kernel/fs/VFSNode.hpp>

namespace kernel {

    class File {
        VFSNode *v_node{};
        off_t position{-1};

        virtual ssize_t read(off_t offset, size_t size, uint8_t *buffer) = 0;

        virtual ssize_t write(off_t offset, size_t size, uint8_t *buffer) = 0;

        virtual void seek(off_t pos) = 0;

    };
}


#endif //OS_CPP_FILE_HPP
