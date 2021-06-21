void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
//用‘；’告诉编译器函数在别的文件中，自己找去吧！
void init_palette(void);
void set_palette(int start, int end, unsigned char* rgb);
void boxfill8(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);
void arc_fill(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int radius, int from_deg, int to_deg);
void radius_box_fill(unsigned char* vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1, int radius);

#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

void HariMain(void)
{
    char* vram;
    int xsize, ysize;
    int bar_width;
    int bar_x;

    init_palette();
    vram = (char*)0xa0000;
    xsize = 320;
    ysize = 200;

    bar_width = 0.8 * xsize;
    bar_x = 0.1 * xsize;

    boxfill8(vram, xsize, COL8_848400, 0, 0, xsize, ysize);

    // boxfill8(vram, xsize, COL8_FFFF00, 30, 50, 80, 100);
    // radius_box_fill(vram, xsize, COL8_C6C6C6, 10, 10, 30, 20, 5);
    // circle_fill(vram, xsize, COL8_C6C6C6, 100, 50, 10);

    // radius_box_fill(vram, xsize, COL8_C6C6C6, bar_x, ysize - 28, xsize - 1 - bar_x, ysize - 28, 5);
    // radius_box_fill(vram, xsize, COL8_FFFFFF, bar_x, ysize - 27, xsize - 1 - bar_x, ysize - 27, 5);
    radius_box_fill(vram, xsize, COL8_C6C6C6, bar_x, ysize - 30, xsize - 1 - bar_x, ysize - 4, 12);

    radius_box_fill(vram, xsize, COL8_FFFFFF, bar_x + 3, ysize - 28, bar_x + 59, ysize - 7, 10);
    // radius_box_fill(vram, xsize, COL8_FFFFFF, bar_x + 2, ysize - 24, bar_x + 2, ysize - 4, 5);
    // radius_box_fill(vram, xsize, COL8_848484, bar_x + 3, ysize - 4, bar_x + 59, ysize - 4, 5);
    // radius_box_fill(vram, xsize, COL8_848484, bar_x + 59, ysize - 23, bar_x + 59, ysize - 5, 5);
    // radius_box_fill(vram, xsize, COL8_000000, bar_x + 2, ysize - 3, bar_x + 59, ysize - 3, 5);
    // radius_box_fill(vram, xsize, COL8_000000, bar_x + 60, ysize - 24, bar_x + 60, ysize - 3, 5);

    // radius_box_fill(vram, xsize, COL8_848484, bar_width - 47, ysize - 24, bar_width - 4, ysize - 24, 5);
    // radius_box_fill(vram, xsize, COL8_848484, bar_width - 47, ysize - 23, bar_width - 47, ysize - 4, 5);
    // radius_box_fill(vram, xsize, COL8_FFFFFF, bar_width - 47, ysize - 3, bar_width - 4, ysize - 3, 5);
    // radius_box_fill(vram, xsize, COL8_FFFFFF, bar_width - 3, ysize - 24, bar_width - 3, ysize - 3, 5);

    for (;;) {
        io_hlt();
    }
}

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
    set_palette(0, 15, table_rgb);
    return;

    /* static char 相当于汇编的DB */
}

void set_palette(int start, int end, unsigned char* rgb)
{
    int i, eflags;
    eflags = io_load_eflags(); /* 记录中断许可标记 */
    io_cli(); /* 清除中断标记 */
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags); /* 复原中断标记 */
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
    for (y = -radius*2; y <= radius*2; y+=0.2) {
        for (x = -radius*2; x <= radius*2; x+=0.2)
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