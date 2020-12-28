//
// Created by Harold on 2020/5/17.
//

#ifndef OS_CPP_FILESYSTEM_HPP
#define OS_CPP_FILESYSTEM_HPP

#include <kernel/fs/VFSNode.hpp>

namespace kernel {
    class FileSystem {
    protected:
        virtual VFSNode *get_node(VFSNodeIndex) = 0;
        VFSNode *mount_point;
    public:
        static void init();
    };

}

#endif //OS_CPP_FILESYSTEM_HPP
