#include "common.h"
#ifndef VGA_HEADERS
#define VGA_HEADERS
#define TEXT_FORE_COLOR_BLACK 0x0
#define TEXT_FORE_COLOR_BLUE 0x1
#define TEXT_FORE_COLOR_GREEN 0x2
#define TEXT_FORE_COLOR_CYAN 0x3
#define TEXT_FORE_COLOR_RED 0x4
#define TEXT_FORE_COLOR_MAGENTA 0x5
#define TEXT_FORE_COLOR_BROWN 0x6
#define TEXT_FORE_COLOR_GRAY 0x7
#define TEXT_FORE_COLOR_GRAY 0x7
#define TEXT_FORE_COLOR_WHITE 0xF

#define TEXT_BACK_COLOR_BLACK 0x0
#define TEXT_BACK_COLOR_BLUE 0x1 << 4
#define TEXT_BACK_COLOR_GREEN 0x2 << 4
#define TEXT_BACK_COLOR_CYAN 0x3 << 4
#define TEXT_BACK_COLOR_RED 0x4 << 4
#define TEXT_BACK_COLOR_MAGENTA 0x5 << 4
#define TEXT_BACK_COLOR_BROWN 0x6 << 4
#define TEXT_BACK_COLOR_GRAY 0xF << 4
#define TEXT_BLINK = 0xF0


void vga_scroll();
void vga_clear();
void vga_putchar_color(char, u8int_t);
void vga_print_color(char *, u8int_t);
void vga_hex_option(u32int_t, u8int_t);
void vga_dec(u32int_t);
#define vga_putchar(x) vga_putchar_color(x, 0xF)
#define vga_print(x) vga_print_color(x, 0xF)
#define vga_hex(x) vga_hex_option(x, 0x0)
#define vga_hex_trim(x) vga_hex_option(x, 0x1)
// #define TEXT_FORE_COLOR_BLUE_LIGHT 0x8
// #define TEXT_FORE_COLOR_GREEN_LIGHT 0x9
// #define TEXT_FORE_COLOR_CYAN_LIGHT 0xA
// #define TEXT_FORE_COLOR_RED_LIGHT 0xB
// #define TEXT_FORE_COLOR_MAGENTA_LIGHT 0xC
// #define TEXT_FORE_COLOR_BLACK_LIGHT 0xD
// #define TEXT_FORE_COLOR_BLACK_LIGHT 0xE

#endif