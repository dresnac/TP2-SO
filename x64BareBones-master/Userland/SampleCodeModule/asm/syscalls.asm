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

section .text

;Mi problema ahora es que si llamo a sys_write con 3 argumentos en vez de 4, me quedan los registros de pasaje de args corridos
;Por eso modifique el syscall
;Ahora rax se asigna en las syscalls específicas, y al llamar a la syscall general me quedan:
;rdi en realidad contiene lo que va en rbx, rsi lo de rcx y rdx lo de rdx (por eso no le asigno nada) 
syscall:

    push rbx
    mov rbx, rdi
    mov rcx, rsi
    int 80h
    pop rbx
    ret


sys_read:
    mov rax, 3
    mov r10, rcx
    call syscall

    ret

sys_write:
    
    mov rax, 4
    call syscall
    
    ret


sys_clear_screen:

    mov rax, 5
    call syscall
  
    ret

sys_zoom:
    mov rax, 7
    call syscall
    
    ret

sys_time:

    mov rax, 8
    call syscall
  
    ret

sys_get_regs:

    mov rax, 9
    call syscall

    ret

sys_put_rectangle:

    push r10
    mov rax, 10
    mov r10, rcx
    call syscall
    pop r10

    ret

sys_get_ticks:

    mov rax, 11
    call syscall
    ret

sys_beep:
    mov rax, 14
    call syscall
    ret