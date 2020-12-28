//
// Created by Harold on 2020/5/14.
//

#ifndef OS_CPP_FAT32_HPP
#define OS_CPP_FAT32_HPP

#include <sys/types.hpp>
#include <sys/gcc.hpp>

#endif //OS_CPP_FAT32_HPP
namespace kernel {

    struct PACKED FAT32_header {
        uint8_t jmp[3];
        char oem[8];
        uint16_t bytes_per_sector;
        uint8_t sectors_per_cluster;
        uint16_t reserved_sector_count;
        uint8_t fat_count;
        uint16_t directory_entry_count;
        uint16_t sector_count_small;
        uint8_t media_type;
        uint16_t sectors_per_fat_small;
        uint16_t sectors_per_track;
        uint16_t head_count;
        uint32_t hidden_sector_count;
        uint32_t sector_count_large;
        // extended attributes
        uint32_t sectors_per_fat_large;
        uint16_t fat_flags;
        uint16_t version;
        uint32_t root_cluster;
        uint16_t fsinfo_sector;
        uint16_t backup_boot_sector;
        char reserved[12];
        uint8_t drive_number;
        uint8_t nt_flags;
        uint8_t signature;
        uint32_t serial;
        char label[11];
        char never_trust[8];
        char bootcode[420];
        uint16_t bootable_sig;
    };
}
