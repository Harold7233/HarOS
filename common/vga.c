#include <common/vga.h>
#include <common/dev.h>
#include <kernel/lock.h>
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
//显卡内存从0xB8000开始
u16int_t *video_memory = (u16int_t *)(0xB8000 + 0xC0000000);
//当前光标的位置
u8int_t cursor_x = 0;
u8int_t cursor_y = 0;
spinlock_t cursor_lock;
static void vga_update_cursor();
// 更新光标位置
static void vga_update_cursor()
{

    u16int_t new_pos = cursor_y * VGA_WIDTH + cursor_x;
    outb(0x3D4, 14);           // IO操作，准备更新高8位
    outb(0x3D5, new_pos >> 8); // 发送高8位
    outb(0x3D4, 15);           // 准备更新低8位
    outb(0x3D5, new_pos);      // 发送低8位
}

// 屏幕上翻
void vga_scroll()
{   
    // 0-24行，如果位于25行说明需要上翻
    if (cursor_y >= VGA_HEIGHT)
    {
        //当前屏幕的所有文字全部上移一行，即framebuffer的内容全部往前移动80个字节，第一行的扔掉
        for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; ++i)
        {
            video_memory[i] = video_memory[i + VGA_WIDTH];
        }

        // 最后一行是空行，并且光标位于此行第一行
        for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++)
        {
            video_memory[i] = ' ' | (TEXT_BACK_COLOR_BLACK | TEXT_FORE_COLOR_WHITE) << 4;
        }
        cursor_y = VGA_HEIGHT - 1;
    }
}

// 输出一个字符
void vga_putchar_color(char c, u8int_t attribute)
{
    char buff[2];
    buff[0] = c;
    buff[1] = '\0';
    vga_print_color(buff, attribute);
}

// 清屏
void vga_clear()
{
    spin_lock(&cursor_lock);
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
    {
        video_memory[i] = 0xF << 4 | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
    vga_update_cursor();
    spin_unlock(&cursor_lock);
}

// 输出一个字符串
void vga_print_color(char *str, u8int_t attribute)
{
    spin_lock(&cursor_lock);
    int i = 0;
    char c;
    while (c = str[i++])
    {
        u16int_t *location;

        // 退格键
        if (c == 0x08 && cursor_x)
        {
            --cursor_x;
        }

        // tab键，x移动到下一个8的倍数
        else if (c == 0x09)
        {
            cursor_x = (cursor_x + 8) & ~(8 - 1);
        }

        else if (c == '\r')
        {
            cursor_x = 0;
        }

        else if (c == '\n')
        {
            cursor_x = 0;
            ++cursor_y;
        }
        else if (c >= ' ')
        {
            location = video_memory + (cursor_y * VGA_WIDTH + cursor_x);
            *location = c | attribute << 8;
            ++cursor_x;
        }

        if (cursor_x >= VGA_WIDTH)
        {
            cursor_x = 0;
            ++cursor_y;
        }
    }
    vga_scroll();
    vga_update_cursor();
    spin_unlock(&cursor_lock);
}
