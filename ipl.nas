; xOS
; TAB=4
		CYLS	EQU		10

		ORG		0x7c00			; 程序转载地址

; FAT12格式软盘

		JMP		entry
		DB		0x90
		DB		"xOS'SIPL"		; 盘符8字节
		DW		512				; 扇区大小，512字节
		DB		1				; 簇大小，1个扇区
		DW		1				; FAT起始位置（扇区）
		DB		2				; FAT的个数
		DW		224				; 根目录大小 一般224
		DW		2880			; 磁盘大小 2880个扇区
		DB		0xf0			; 磁盘种类
		DW		9				; FAT长度 必须为9
		DW		18				; 1个磁道有多少个扇区（18）
		DW		2				; 磁头数 2
		DD		0				; 不使用分区 0
		DD		2880			; 再写一遍磁盘大小
		DB		0,0,0x29		; 意义不明，固定
		DD		0xffffffff		; 卷标号码
		DB		"xOS        "	; 磁盘名称（11）
		DB		"FAT12   "		; 磁盘格式名称（8）
		RESB	18				; 空出18个字节

; 核心

entry:
		MOV		AX,0			; 初始化寄存器
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

; 读盘

		MOV 	AX,0x0820		; 0x8000 ~ 0x81FF是留给启动区的
		MOV		ES,AX
		MOV		CH,0			; 柱面0
		MOV		CL,2			; 扇区2
		MOV		DH,0			; 磁头0
readloop:
		MOV		SI,0			; 记录失败次数
retry:
		MOV		AH,0x02			; 读盘
		MOV		AL,1			; 读取1个扇区
		MOV		BX,0			; 
		MOV		DL,0x00			; A驱动器
		INT		0x13			; 调用磁盘BIOS
		JNC		next
		; 出错管理
		CMP		SI, 4			
		JAE		ERROR
		; Reset
		MOV		AH,0x00			; Reset指令
		MOV		DL,0x00			; A驱动器
		INT		0x13			; 调用磁盘BIOS
		JMP		retry
next:
		MOV		AX,ES
		ADD		AX,0x0020
		MOV		ES,AX
		ADD		CL,1	
		CMP		CL,18
		JBE		readloop	; 小于等于18就循环readloop
		MOV		CL,1
		ADD		DH,1		; 切换到柱头1
		CMP		DH,2
		JB		readloop	; 如果读头小于2就……
		MOV		DH,0		; 切换到柱头0
		ADD		CH,1
		CMP		CH,CYLS
		JB		readloop	; 如果柱面小于柱面常量就……


fin:
		HLT						; 让CPU停止；等待指令
		JMP		fin				; 无限循环

ERROR:
		MOV		SI,error_msg
		MOV		CX,16

putloop:
		MOV		AL,[SI]
		ADD		SI,1			; 给SI+1
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 显示一个文字
		MOV		BX,CX			; 指定字符颜色
		INC     CX
		INT		0x10			; 调用显卡BIOS
		JMP		putloop


error_msg:
		DB		0x0a, 0x0a		; 换行两次
		DB		"xOS: read failed."
		DB		0x0a			; 换行
		DB		0

welcome_msg:
		DB		0x0a, 0x0a		; 换行两次
		DB		"xOS: hello."
		DB		0x0a			; 换行
		DB		0

		RESB	0x7dfe-$		; 重复0

		DB		0x55, 0xaa
