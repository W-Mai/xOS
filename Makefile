
# 

TOOLPATH = ../z_tools/
MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
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

xOS.sys : xOS.nas Makefile
	$(NASK) xOS.nas xOS.sys xOS.lst

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
	-$(DEL) ipl.bin
	-$(DEL) ipl.lst
	-$(DEL) xOS.sys
	-$(DEL) xOS.lst

src_only :
	$(MAKE) clean
	-$(DEL) xOS.img
