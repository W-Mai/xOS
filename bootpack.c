void io_hlt(void);

//用‘；’告诉编译器函数在别的文件中，自己找去吧！

void HariMain(void)
{

fin:
    io_hlt(); //执行naskfunc.nas里的io_hlt
    goto fin;
    
} 