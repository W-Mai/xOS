
# 

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/
MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
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
bootpack.gas : bootpack.c Makefile
	$(CC1) -o bootpack.gas bootpack.c

bootpack.nas : bootpack.gas Makefile
	$(GAS2NASK) bootpack.gas bootpack.nas

bootpack.obj : bootpack.nas Makefile
	$(NASK) bootpack.nas bootpack.obj bootpack.lst

bootpack.bim : bootpack.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		bootpack.obj
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

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
	-$(DEL) haribote.sys

src_only :
	$(MAKE) clean
	-$(DEL) xOS.img
