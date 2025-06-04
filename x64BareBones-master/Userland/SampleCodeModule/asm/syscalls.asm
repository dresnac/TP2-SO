GLOBAL syscall
GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_clear_screen
GLOBAL sys_time
GLOBAL sys_get_regs
GLOBAL sys_zoom
GLOBAL sys_put_rectangle
GLOBAL sys_get_ticks
GLOBAL sys_input
GLOBAL sys_read_input
GLOBAL sys_beep
GLOBAL sys_test_mm

section .text

%macro pushState 0
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro popState 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

%macro pushStateMinusRax 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi

%endmacro

%macro popStateMinusRax 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
%endmacro

%macro sys_interrupt 1
        mov rax, %1
        int 80h
%endmacro

;syscall:

;    push rbx
;    mov rbx, rdi
;    mov rcx, rsi
;    int 80h
;    pop rbx
;    ret

%macro syscall 1
    push rbp
    mov rbp, rsp
    mov rax, %1
    int 80h
    mov rsp, rbp
    pop rbp
    ret
%endmacro


sys_read:
    syscall 0
    ;mov rax, 3
    ;mov r10, rcx
    ;call syscall

    ;ret

sys_write:
    syscall 1
    ;mov rax, 4
    ;call syscall
    
    ;ret


sys_clear_screen:
    syscall 2
    ;mov rax, 5
    ;call syscall
  
    ret

sys_zoom:
    ;mov rax, 7
    ;call syscall
    
    ret

sys_time:

    ;mov rax, 8
    ;call syscall
  
    ;ret

sys_get_regs:

    ;mov rax, 9
    ;call syscall

    ret

sys_put_rectangle:

    ;push r10
    ;mov rax, 10
    ;mov r10, rcx
    ;call syscall
    ;pop r10

    ret

sys_get_ticks:

    ;mov rax, 11
    ;call syscall
    ret

sys_beep:
    ;mov rax, 14
    ;call syscall
    ret

sys_test_mm:
    ;mov rax, 12
    ;call syscall
    ret