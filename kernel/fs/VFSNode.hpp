//
// Created by Harold on 2020/5/8.
//

#ifndef OS_CPP_VFSNODE_HPP
#define OS_CPP_VFSNODE_HPP

#include <kernel/time.hpp>
#include <libcpp/String.hpp>

namespace kernel {
    constexpr uint32_t STDOUT_NODE_NO = 0;
    constexpr uint32_t STDIN_NODE_NO = 1;
    constexpr uint32_t STDERR_NODE_NO = 2;
    constexpr uint32_t ROOT_NODE_NO = 3;

    class VFSNode;

    class Device;

    class CharacterDevice;

    class BlockDevice;

    class FileSystem;

    class VFSNodeIndex {
        friend class VFSNode;

        friend class FileSystem;

        uint32_t index;
        const char *name;

    };

    class VFSDirectoryEntry {
        friend class VFSNode;

        char *name{};
        VFSNode *node{};
        VFSNodeIndex index{};
    };

    class VFSNode {
    public:
        time_t c_time{};
        time_t m_time{};
        time_t a_time{};
        uint16_t file_type{};
        size_t size{}; // 文件大小（字节数）
        FileSystem *fs{};
        union {
            CharacterDevice *char_device;
            BlockDevice *block_device;
        } device{};

        bool is_mount_point();

        bool mounted();

        VFSNode() = default;

        ssize_t read(off_t start, size_t bytes, uint8_t *buffer);

        ssize_t write(off_t start, size_t bytes, uint8_t *buffer);

        virtual VFSNode *lookup(String child) = 0;

        virtual bool readable();

        virtual bool writable();

        virtual bool is_file();

        virtual bool is_directory();

        virtual bool is_block_device();

        virtual bool is_character_device();


    };

    extern VFSNode *node_table[1024];
}

#endif //OS_CPP_VFSNODE_HPP
