//
// Created by Harold on 2020/5/17.
//

#include <kernel/fs/ext2/Ext2FS.hpp>
#include <libcpp/cmath.hpp>
#include <libcpp/cstring.hpp>
#include <kernel/stdio.hpp>

namespace kernel {
    Ext2FS::Ext2FS(BlockDevice &device) : BlockFileSystem(device) {
        auto *buffer = reinterpret_cast<uint8_t *>(&super_block_);
        BlockIndex super_block_index = 1024 / device.block_size();
        auto result = device.block_read(super_block_index, sizeof(super_block_) / device_.block_size(), buffer);
        if (result < 0) {
            // TODO fail
            PANIC("ext2 init failed!");
        }
        if (super_block_.s_magic != EXT2_SUPER_MAGIC) {
            // TODO fail
            PANIC("ext2 magic not matched!");
        }
        block_size_ = super_block_.s_log_block_size;
        /*  https://wiki.osdev.org/Ext2#Determining_the_Number_of_Block_Groups
         *  From the Superblock, extract the size of each block, the total number of inodes,
         *  the total number of blocks, the number of blocks per block group,
         *  and the number of inodes in each block group.
         *  From this information we can infer the number of block groups there are by:
            Rounding up the total number of blocks divided by the number of blocks per block group
            Rounding up the total number of inodes divided by the number of inodes per block group
            Both (and check them against each other)
         */
        block_size_ = EXT2_BLOCK_SIZE(&super_block_);
        block_group_count_ = ceil_divide(super_block_.s_blocks_count, super_block_.s_blocks_per_group);
        size_t count = ceil_divide(block_group_count_ * sizeof(ext2_group_desc), block_size_);
        auto *buf = new uint8_t[count * block_size_];
        /*
         * The table is located in the block immediately following the Superblock.
         * So if the block size (determined from a field in the superblock) is 1024 bytes per block,
         * the Block Group Descriptor Table will begin at block 2.
         * For any other block size, it will begin at block 1.
         * Remember that blocks are numbered starting at 0,
         * and that block numbers don't usually correspond to physical block addresses.
         */
        auto bgdt_size_in_bytes = block_group_count_ * sizeof(ext2_group_desc);
        bgdt = new ext2_group_desc[block_group_count_];
        BlockIndex block_gdt_start = block_size_ == 1024 ? 2 : 1;
        block_read(block_gdt_start, count, buf);
        memcpy(bgdt, buf, bgdt_size_in_bytes);
        delete[] buf;
    }


    ext2_inode *Ext2FS::get_inode(inode_index_t index) {
        auto grp = (index - 1) / super_block_.s_inodes_per_group;
        auto table_index = (index - 1) % super_block_.s_inodes_per_group;
        BlockIndex containing_block = bgdt[grp].bg_inode_table + (index * super_block_.s_inode_size / block_size_);
        uint8_t buffer[1024];
        block_read(containing_block, 1, buffer);
        auto offset = (table_index * super_block_.s_inode_size) % block_size_;
        auto *inode = new ext2_inode;
        memcpy(inode, buffer + offset, sizeof(ext2_inode));
        return inode;
    }

    ext2_inode *Ext2FS::find_dir_entry(const ext2_inode *inode, const char *filename) {
        return nullptr;
    }

    ssize_t Ext2FS::traverse_dir(ext2_inode *inode) {
        if (!S_ISDIR(inode->i_mode)) {
            return -1; // TODO
        }
        ssize_t count = 0;
        DataBlockAddressing a(*this, const_cast<ext2_inode *>(inode));
        BlockIndex dir_entry_block_i = a.data_block_index(0);
        uint8_t buffer[1024];
        size_t size = 0;
        size_t block_count = ceil_divide(inode->i_size, block_size_);
        for (size_t i = 0; i < block_count; ++i) {
            block_read(dir_entry_block_i, 1, buffer);
            while (size < (i + 1) * block_size_ && size < inode->i_size) {
                auto *entry = reinterpret_cast<ext2_dir_entry_2 *>(buffer + size % block_size_);
                uint8_t filename[EXT2_NAME_LEN];
                memcpy(filename, entry->name, entry->name_len);
                filename[entry->name_len] = 0;
                kprint("Filename: %s\n", filename);
                if (count >= 2) {
                    traverse_dir(get_inode(entry->inode));
                }
                size += entry->rec_len;
                ++count;
            }
        }
        return count;
    }


    ssize_t Ext2FS::read_data_blocks(const ext2_inode *inode, BlockIndex start, ssize_t count, uint8_t *buffer) {

    }

    VFSNode *Ext2FS::get_node(VFSNodeIndex) {
        return nullptr;
    }

    void DataBlockAddressing::update() {
        int i = 0;
        for (; i < 12; ++i) {
            if (inode->i_block[i] == 0) {
                break;
            }
        }
        BlockIndex singly_table_addr = inode->i_block[12];
        if (i < 11 || singly_table_addr) {
            direct_only = true;
            singly_only = doubly_only = triply_only = false;
            return;
        }
        direct_only = false;
        singly = new BlockIndex[fs.block_size_];
        fs.block_read(singly_table_addr, 1, reinterpret_cast<uint8_t *>(singly));
        BlockIndex doubly_table_addr = inode->i_block[13];
        if (!doubly_table_addr) {
            singly_only = true;
            doubly_only = triply_only = false;
            return;
        }
        doubly = new BlockIndex[fs.block_size_];
        fs.block_read(doubly_table_addr, 1, reinterpret_cast<uint8_t *>(doubly));

    }

}