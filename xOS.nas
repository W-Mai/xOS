; xOS
; TAB=4

    ORG     0xC200

    MOV     AH,0x00
    MOV     AL,0x13
    INT     0x10

fin:
    HLT						; 让CPU停止；等待指令
    JMP		fin				; 无限循环