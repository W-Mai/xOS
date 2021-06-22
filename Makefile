OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj \
		int.obj

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
QEMUPATH = ..\z_tools\qemu
COPY     = copy
DEL      = del

default :
	$(MAKE) img

# 文件生成规则



ipl.bin : ipl.nas Makefile
	$(NASK) ipl.nas ipl.bin ipl.lst

xOS.bin : xOS.nas Makefile
	$(NASK) xOS.nas xOS.bin xOS.lst

#将bootpack.c变成机器语言
#首先，使用ccl.exc从bootpack.c生 成bootpack.gas。
%.gas : %.c Makefile
	$(CC1) -o $*.gas $*.c

#第二步，使用gas2nask.exe从bootpack-gas生成bootpack.nas。
%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

#第三步，使用nask.exc从bootpack nas生 成bootpack.obj。
%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

#构建字体文件
hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

#增加naskfunc
naskfunc.obj : naskfunc.nas Makefile
	$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst

#第四步，使用obi2bim.exe从bootpack.obj生成bootpack.bim。
bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

#最后，使用bim2hrb.exe从boopack bim生成boopack.hrb。
bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

#这样就做成了机器语言，再使用copy指令将xOS.bin 与bootpack.brb单
#就成了xOS.sys。
xOS.sys : xOS.bin bootpack.hrb Makefile
	copy /B xOS.bin+bootpack.hrb xOS.sys

xOS.img : ipl.bin xOS.sys Makefile
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl.bin len:512 from:0 to:0 \
		copy from:xOS.sys to:@: \
		imgout:xOS.img

# 命令

img :
	$(MAKE) xOS.img

run :
	$(MAKE) img
	$(COPY) xOS.img $(QEMUPATH)\fdimage0.bin
	$(MAKE) -C $(QEMUPATH)

install :
	$(MAKE) img
	$(IMGTOL) w a: xOS.img

clean :
	-$(DEL) xOS.sys
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) bootpack.nas
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) bootpack.hrb

src_only :
	$(MAKE) clean
	-$(DEL) xOS.img
