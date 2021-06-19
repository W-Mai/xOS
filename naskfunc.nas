; naskfunc
; TAB=4

[FORMAT "WCOFF"]             ; 制作目标文件的模式
[INSTRSET "i486p"]
[BITS 32]		             ; 制作32位模式用的机械语言


; 制作目标文件的信息

[FILE "naskfunc.nas"]		 ; 源文件名信息
		GLOBAL	_io_hlt,_write_mem8		 ; 程序中包含的函数名


; 以下是实际的函数

[SECTION .text]		; 目标文件中写了这些之后再写程序

_io_hlt:	; void io_hlt(void)；
	HLT
	RET

_write_mem8:	; void write_mem8(int addr, int data);
		MOV		ECX,[ESP+4]		; [ESP+4]存地址
		MOV		AL,[ESP+8]		; [ESP+8]存数据
		MOV		[ECX],AL
		RET
