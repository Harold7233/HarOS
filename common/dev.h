#ifndef DEV_H
#define DEV_H
#include <common/common.h>
#include <libc/stdarg.h>
#include <common/vga.h>
void printk(const char *, ...);
void printk_color(u8int_t attribute, const char *, ...);
enum emPANIC
{
    OUT_OF_FRAMES = 1
};


#endif