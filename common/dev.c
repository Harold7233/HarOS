#include <common/common.h>
#include <libc/string.h>
#include <common/vga.h>
#include <common/dev.h>

static void printk_color_v(u8int_t, const char *, va_list);
void printk(const char *fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    printk_color_v(TEXT_BACK_COLOR_BLACK | TEXT_FORE_COLOR_WHITE, fmt, list);
    va_end(list);
}

void printk_color(u8int_t attribute, const char *fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    printk_color_v(attribute, fmt, list);
    va_end(list);
}

static void printk_color_v(u8int_t attribute, const char *fmt, va_list list)
{

    char buffer[256]; // 设置成1024的话在系统调用中会挂掉？
    vsprintf(buffer, fmt, list);
    vga_print_color(buffer, attribute);
}

void panic_detail(char *msg, char *file, u32int_t line, u8int_t color)
{
    printk_color(color, msg);
    if (file && line)
    {
        printk_color(color, " (File: %s, Line: %d)\n", file, line);
    }

    halt();
}