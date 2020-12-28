//
// Created by Harold on 2020/5/17.
//

#ifndef OS_CPP_EXT2FS_HPP
#define OS_CPP_EXT2FS_HPP

#include <kernel/fs/ext2/ext2_fs.h>
#include <kernel/fs/BlockFileSystem.hpp>
#include <kernel/fs/VFSNode.hpp>
#include <libcpp/assert.hpp>

#define EXT2_S_IFSOCK    0xC000 //socket
#define EXT2_S_IFLNK    0xA000 //symbolic link
#define EXT2_S_IFREG    0x8000 //regular file
#define EXT2_S_IFBLK    0x6000 //block device
#define EXT2_S_IFDIR    0x4000 //directory
#define EXT2_S_IFCHR    0x2000 //character device
#define EXT2_S_IFIFO    0x1000 //fifo
//-- process execution user/group override --
#define EXT2_S_ISUID    0x0800 //Set process User ID
#define EXT2_S_ISGID    0x0400 //Set process Group ID
#define EXT2_S_ISVTX    0x0200 //sticky bit
//-- access rights --
#define EXT2_S_IRUSR    0x0100 //user read
#define EXT2_S_IWUSR    0x0080 //user write
#define EXT2_S_IXUSR    0x0040 //user execute
#define EXT2_S_IRGRP    0x0020 //group read
#define EXT2_S_IWGRP    0x0010 //group write
#define EXT2_S_IXGRP    0x0008 //group execute
#define EXT2_S_IROTH    0x0004 //others read
#define EXT2_S_IWOTH    0x0002 //others write
#define EXT2_S_IXOTH    0x0001 //others execute

#define S_IFMT  00170000
#define S_IFSOCK 0140000
#define S_IFLNK     0120000
#define S_IFREG  0100000
#define S_IFBLK  0060000
#define S_IFDIR  0040000
#define S_IFCHR  0020000
#define S_IFIFO  0010000
#define S_ISUID  0004000
#define S_ISGID  0002000
#define S_ISVTX  0001000

#define S_ISLNK(m)    (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)    (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)    (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)    (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)    (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)    (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)    (((m) & S_IFMT) == S_IFSOCK)


namespace kernel {
    typedef uint32_t inode_index_t;

    typedef bool (*ext2_traverse_cb_t)(ext2_inode *);

    class Ext2FS;

    class DataBlockAddressing {
        friend class Ext2FS;

        ext2_inode *inode;
        Ext2FS &fs;
        bool direct_only{false};
        bool singly_only{false};
        bool doubly_only{false};
        bool triply_only{false};
        BlockIndex *singly{};
        BlockIndex *doubly{};
        BlockIndex *triply{};


        explicit DataBlockAddressing(Ext2FS &fs, ext2_inode *inode) : inode(inode), fs(fs) {

        }

        void update();

        BlockIndex data_block_index(BlockIndex linear) {
            if (singly_only) {
                assert(linear < 12);
            }
            if (linear < 12) {
                return inode->i_block[linear];
            }
            if (doubly_only) {
//                assert(linear < 11 + )
                return doubly[linear - 12];
            }
            // TODO

        }
    };

    class Ext2Node : public VFSNode {
    public:
        Ext2Node(ext2_inode &inode) {
            c_time = inode.i_ctime;
            a_time = inode.i_dtime;
        }
    };

    class Ext2FS : public BlockFileSystem {
        friend class DataBlockAddressing;

    public:
        size_t block_group_count_{};
        ext2_group_desc *bgdt{};

        ext2_inode *get_inode(inode_index_t);

        ext2_inode *get_root_node() {
            return get_inode(EXT2_ROOT_INO);

        }

        ssize_t traverse_dir(ext2_inode *inode);

        ext2_inode *find_dir_entry(const ext2_inode *inode, const char *filename = nullptr);

        ssize_t read_data_blocks(const ext2_inode *inode, BlockIndex start, ssize_t count, uint8_t *buffer);

    public:
        ext2_super_block super_block_{};

        VFSNode *get_node(VFSNodeIndex) override;

        explicit Ext2FS(BlockDevice &device);
    };


}

#endif //OS_CPP_EXT2FS_HPP
