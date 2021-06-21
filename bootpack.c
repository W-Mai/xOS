void io_hlt(void);
void write_mem8(int addr, int data);

//用‘；’告诉编译器函数在别的文件中，自己找去吧！

void HariMain(void)
{
    int i, j;

loop:
    for (j = 1; j < 16; j++) {
        for (i = 0xa0000; i <= 0xaffff; i++) { //i表示像素点的地址，数字代表显示的颜色。15白色
            write_mem8(i, i % j); //数字替换为‘i&15’可以显示彩色条纹
        }
        io_hlt();
    }
    goto loop;

    for (;;) {
        io_hlt();
    }
}
