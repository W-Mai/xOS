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
void init_screen8(char* vram, int x, int y);
void putfont8(char* vram, int xsize, int x, int y, char c, char* font);
void putfonts8_asc(char* vram, int xsize, int x, int y, char c, unsigned char* s);
void init_mouse_cursor8(char* mouse, char bc);
void putblock8_8(char* vram, int vxsize, int pxsize,
    int pysize, int px0, int py0, char* buf, int bxsize);

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

struct BOOTINFO {
    char cyls, leds, vmode, reserve;
    short scrnx, scrny;
    char* vram;
};

struct SEGMENT_DESCRIPTOR {
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};

void init_gdtidt(void);
void set_segmdesc(struct SEGMENT_DESCRIPTOR* sd, unsigned int limit, int base, int ar);
void set_gatedesc(struct GATE_DESCRIPTOR* gd, int offset, int selector, int ar);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);

void HariMain(void)
{

    struct BOOTINFO* binfo = (struct BOOTINFO*)0x0ff0;
    char s[40], mcursor[256];
    int mx, my;

    init_gdtidt();
    init_palette();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
    mx = (binfo->scrnx - 16) / 2;
    my = (binfo->scrny - 28 - 16) / 2;
    init_mouse_cursor8(mcursor, COL8_008484);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
    sprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);



    // vram = (char*)0xe0000000;
    // xsize = 1024;
    // ysize = 768;

    // bar_width = 0.8 * xsize;
    // bar_x = 0.1 * xsize;

    // boxfill8(vram, xsize, COL8_848400, 0, 0, xsize, ysize);

    // boxfill8(vram, xsize, COL8_FFFF00, 30, 50, 80, 100);
    // radius_box_fill(vram, xsize, COL8_C6C6C6, 10, 10, 30, 20, 5);
    // circle_fill(vram, xsize, COL8_C6C6C6, 100, 50, 10);

    // // radius_box_fill(vram, xsize, COL8_C6C6C6, bar_x, ysize - 28, xsize - 1 - bar_x, ysize - 28, 5);
    // // radius_box_fill(vram, xsize, COL8_FFFFFF, bar_x, ysize - 27, xsize - 1 - bar_x, ysize - 27, 5);
    // radius_box_fill(vram, xsize, COL8_C6C6C6, bar_x, ysize - 50, xsize - 1 - bar_x, ysize - 4, 10);

    // radius_box_fill(vram, xsize, COL8_FFFFFF, bar_x + 10, ysize - 40, bar_x + 60, ysize - 14, 4);
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
    for (y = -radius*2; y <= radius*2; y+=1) {
        for (x = -radius*2; x <= radius*2; x+=1)
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
        if ((d & 0x80) != 0) {
            p[0] = c;
        }
        if ((d & 0x40) != 0) {
            p[1] = c;
        }
        if ((d & 0x20) != 0) {
            p[2] = c;
        }
        if ((d & 0x10) != 0) {
            p[3] = c;
        }
        if ((d & 0x08) != 0) {
            p[4] = c;
        }
        if ((d & 0x04) != 0) {
            p[5] = c;
        }
        if ((d & 0x02) != 0) {
            p[6] = c;
        }
        if ((d & 0x01) != 0) {
            p[7] = c;
        }
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
        ".......**.......",
        "....***OO***....",
        "...*OOOOOOOO*...",
        "..*OOOOOOOOOO*..",
        ".*OOOOOOOOOOOO*.",
        ".*OOOOO**OOOOO*.",
        ".*OOOO*..*OOOO*",
        "*OOOO*....*OOOO*",
        "*OOOO*....*OOOO*",
        ".*OOOO*..*OOOO*.",
        ".*OOOOO**OOOOO*.",
        ".*OOOOOOOOOOOO*.",
        "..*OOOOOOOOOO*..",
        "...*OOOOOOOO*...",
        "....***OO***....",
        ".......**.......",
    };
    int x, y;

    for (y = 0; y < 16; y++) {
        for (x = 0; x < 16; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * 16 + x] = COL8_000000;
            }
            if (cursor[y][x] == 'O') {
                mouse[y * 16 + x] = COL8_FFFFFF;
            }
            if (cursor[y][x] == '.') {
                mouse[y * 16 + x] = bc;
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

void init_gdtidt(void)
{
    struct SEGMENT_DESCRIPTOR* gdt = (struct SEGMENT_DESCRIPTOR*)0x00270000;
    struct GATE_DESCRIPTOR* idt = (struct GATE_DESCRIPTOR*)0x0026f800;
    int i;

    /* GDT */
    for (i = 0; i < 8192; i++) {
        set_segmdesc(gdt + i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);
    set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);
    load_gdtr(0xffff, 0x00270000);

    /* IDT */
    for (i = 0; i < 256; i++) {
        set_gatedesc(idt + i, 0, 0, 0);
    }
    load_idtr(0x7ff, 0x0026f800);

    return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR* sd, unsigned int limit, int base, int ar)
{
    if (limit > 0xfffff) {
        ar |= 0x8000; /* G_bit = 1 */
        limit /= 0x1000;
    }
    sd->limit_low = limit & 0xffff;
    sd->base_low = base & 0xffff;
    sd->base_mid = (base >> 16) & 0xff;
    sd->access_right = ar & 0xff;
    sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->base_high = (base >> 24) & 0xff;
    return;
}

void set_gatedesc(struct GATE_DESCRIPTOR* gd, int offset, int selector, int ar)
{
    gd->offset_low = offset & 0xffff;
    gd->selector = selector;
    gd->dw_count = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high = (offset >> 16) & 0xffff;
    return;
}
