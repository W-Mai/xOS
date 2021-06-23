/* 显示 */

#include "bootpack.h"

void init_palette(void)
{
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00,
        0xff, 0x00, 0x00,
        0x00, 0xff, 0x00,
        0xff, 0xff, 0x00,
        0x00, 0x00, 0xff,
        0xff, 0x00, 0xff,
        0x00, 0xff, 0xff,
        0xff, 0xff, 0xff,
        0xc6, 0xc6, 0xc6,
        0x84, 0x00, 0x00,
        0x00, 0x84, 0x00,
        0x84, 0x84, 0x00,
        0x00, 0x00, 0x84,
        0x84, 0x00, 0x84,
        0x00, 0x84, 0x84,
        0x84, 0x84, 0x84
    };
    unsigned char table2[216 * 3];
    int r, g, b;
    set_palette(0, 15, table_rgb);
    for (b = 0; b < 6; b++) {
        for (g = 0; g < 6; g++) {
            for (r = 0; r < 6; r++) {
                table2[(r + g * 6 + b * 36) * 3 + 0] = r * 51;
                table2[(r + g * 6 + b * 36) * 3 + 1] = g * 51;
                table2[(r + g * 6 + b * 36) * 3 + 2] = b * 51;
            }
        }
    }
    set_palette(16, 231, table2);
    return;

    /* static char 相当于汇编的DB */
}

void set_palette(int start, int end, unsigned char* rgb)
{
    int i, eflags;
    eflags = io_load_eflags(); 
    io_cli(); 
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags);
    return;
}

void boxfill8(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
    int x, y;
    for (y = y0; y <= y1; y++) {
        for (x = x0; x <= x1; x++)
            vram[y * xsize + x] = c;
    }
    return;
}

void circle_fill(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int radius)
{
    double x, y;
    for (y = -radius * 2; y <= radius * 2; y += 1) {
        for (x = -radius * 2; x <= radius * 2; x += 1)
            if (x * x + y * y <= radius * radius) {
                vram[(int)(y + y0) * xsize + (int)x + x0] = c;
            }
    }
    return;
}

void radius_box_fill(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1, int radius)
{
    circle_fill(vram, xsize, c, x0 + radius, y0 + radius, radius);
    circle_fill(vram, xsize, c, x1 - radius, y0 + radius, radius);
    circle_fill(vram, xsize, c, x0 + radius, y1 - radius, radius);
    circle_fill(vram, xsize, c, x1 - radius, y1 - radius, radius);

    boxfill8(vram, xsize, c, x0 + radius, y0 + radius, x1 - radius, y1 - radius); // 主体
    boxfill8(vram, xsize, c, x0 + radius, y0, x1 - radius, y0 + radius); // 上
    boxfill8(vram, xsize, c, x1 - radius, y0 + radius, x1, y1 - radius); // 右
    boxfill8(vram, xsize, c, x0 + radius, y1 - radius, x1 - radius, y1); // 下
    boxfill8(vram, xsize, c, x0, y0 + radius, x0 + radius, y1 - radius); // 左
}

void init_screen8(char* vram, int x, int y)
{
    int xsize = x, ysize = y;
    int bar_width;
    int bar_x;

    bar_width = 0.8 * xsize;
    bar_x = 0.1 * xsize;

    boxfill8(vram, xsize, COL8_008484, 0, 0, xsize, ysize);

    boxfill8(vram, xsize, COL8_FFFF00, 30, 50, 80, 100);
    radius_box_fill(vram, xsize, COL8_C6C6C6, 10, 10, 30, 20, 5);
    circle_fill(vram, xsize, COL8_C6C6C6, 100, 50, 10);

    radius_box_fill(vram, xsize, COL8_C6C6C6, bar_x, ysize - 50, xsize - 1 - bar_x, ysize - 4, 10);

    radius_box_fill(vram, xsize, COL8_FFFFFF, bar_x + 10, ysize - 40, bar_x + 60, ysize - 14, 4);

    return;
}

void putfont8(char* vram, int xsize, int x, int y, char c, char* font)
{
    int i;
    char *p, d /* data */;
    for (i = 0; i < 16; i++) {
        p = vram + (y + i) * xsize + x;
        d = font[i];
        if ((d & 0x80) != 0) p[0] = c;
        if ((d & 0x40) != 0) p[1] = c;
        if ((d & 0x20) != 0) p[2] = c;
        if ((d & 0x10) != 0) p[3] = c;
        if ((d & 0x08) != 0) p[4] = c;
        if ((d & 0x04) != 0) p[5] = c;
        if ((d & 0x02) != 0) p[6] = c;
        if ((d & 0x01) != 0) p[7] = c;
    }
    return;
}

void putfonts8_asc(char* vram, int xsize, int x, int y, char c, unsigned char* s)
{
    extern char hankaku[4096];
    for (; *s != 0x00; s++) {
        putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
        x += 8;
    }
    return;
}

void init_mouse_cursor8(char* mouse, char bc)
{
    static char cursor[16][16] = {
        "......****......",
        "....**OOOO**....",
        "...*OOOOOOOO*...",
        "..*OOOOOOOOOO*..",
        ".*OOOOOOOOOOOO*.",
        ".*OOOOO**OOOOO*.",
        "*OOOOO*..*OOOOO*",
        "*OOOO*....*OOOO*",
        "*OOOO*....*OOOO*",
        "*OOOOO*..*OOOOO*",
        ".*OOOOO**OOOOO*.",
        ".*OOOOOOOOOOOO*.",
        "..*OOOOOOOOOO*..",
        "...*OOOOOOOO*...",
        "....**OOOO**....",
        "......****......",
    };
    int x, y;
    int index;
    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            index = y * 16 + x;
            if (cursor[y][x] == '*') {
                mouse[index] = COL8_000000;
            }
            if (cursor[y][x] == 'O') {
                mouse[index] = COL8_FFFFFF;
            }
            if (cursor[y][x] == '.') {
                mouse[index] = bc;
            }
        }
    }
    return;
}

void putblock8_8(char* vram, int vxsize, int pxsize,
    int pysize, int px0, int py0, char* buf, int bxsize)
{
    int x, y;
    for (y = 0; y < pysize; y++) {
        for (x = 0; x < pxsize; x++) {
            vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
        }
    }
    return;
}
