section .text
global opCode

opCode:
    push rbp
    mov rbp, rsp

    jmp 0

    mov rsp, rbp
    pop rbp
    ret