GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_get_register_snapshot
GLOBAL sys_beep
GLOBAL sys_set_font_size
GLOBAL sys_clear_screen
GLOBAL sys_put_pixel
GLOBAL sys_put_rectangle
GLOBAL sys_draw_letter
GLOBAL sys_set_mode
GLOBAL sys_get_screen_information
GLOBAL sys_ticks_sleep
GLOBAL sys_get_time
GLOBAL sys_free
GLOBAL sys_malloc
GLOBAL sys_get_pid
GLOBAL sys_create_process
GLOBAL sys_block
GLOBAL sys_yield
GLOBAL sys_unblock
GLOBAL sys_nice
GLOBAL sys_kill
GLOBAL sys_wait
GLOBAL sys_sem_open
GLOBAL sys_sem_wait
GLOBAL sys_sem_post
GLOBAL sys_sem_close
GLOBAL sys_ps
GLOBAL sys_free_ps
GLOBAL sys_get_status
GLOBAL sys_pipe_open
GLOBAL sys_pipe_open_free
GLOBAL sys_pipe_read
GLOBAL sys_pipe_write
GLOBAL sys_pipe_close
GLOBAL sys_pipe_reserve
GLOBAL sys_sem_open_get_id
GLOBAL sys_get_my_fds
GLOBAL sys_mem_info

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

%macro syscall_handler 1
    push rbp
    mov rbp, rsp
    mov rax, %1
    int 80h
    mov rsp, rbp
    pop rbp
    ret
%endmacro


sys_read:
    syscall_handler 0

sys_write:
    syscall_handler 1

sys_get_register_snapshot:
    syscall_handler 2

sys_beep:
    syscall_handler 3

sys_set_font_size:
    syscall_handler 4

sys_clear_screen:
    syscall_handler 5

sys_put_pixel:
    syscall_handler 6

sys_put_rectangle:
    syscall_handler 7

sys_draw_letter:
    syscall_handler 8

sys_set_mode:
    syscall_handler 9

sys_get_screen_information:
    syscall_handler 10

sys_ticks_sleep:
    syscall_handler 11

sys_get_time:
    syscall_handler 12

sys_malloc:
    syscall_handler 13

sys_free:
    syscall_handler 14

sys_get_pid:
    syscall_handler 15

sys_create_process:
    syscall_handler 16

sys_block:
    syscall_handler 17

sys_unblock:
    syscall_handler 18

sys_yield:
    syscall_handler 19

sys_nice:
    syscall_handler 20

sys_kill:
    syscall_handler 21

sys_wait:
    syscall_handler 22

sys_sem_open:
    syscall_handler 23

sys_sem_wait:
    syscall_handler 24

sys_sem_post:
    syscall_handler 25

sys_sem_close:
    syscall_handler 26

sys_ps:
    syscall_handler 27

sys_free_ps:
    syscall_handler 28

sys_get_status:
    syscall_handler 29

sys_pipe_open:
    syscall_handler 30

sys_pipe_open_free:
    syscall_handler 31

sys_pipe_read:
    syscall_handler 32

sys_pipe_write:
    syscall_handler 33

sys_pipe_close:
    syscall_handler 34

sys_pipe_reserve:
    syscall_handler 35

sys_sem_open_get_id:
    syscall_handler 36

sys_get_my_fds:
    syscall_handler 37

sys_mem_info:
    syscall_handler 38

;----------BACKUP--------------

; GLOBAL syscall
; GLOBAL sys_read
; GLOBAL sys_write
; GLOBAL sys_clear_screen
; GLOBAL sys_time
; GLOBAL sys_get_regs
; GLOBAL sys_zoom
; GLOBAL sys_put_rectangle
; GLOBAL sys_get_ticks
; GLOBAL sys_input
; GLOBAL sys_read_input
; GLOBAL sys_beep
; GLOBAL sys_test_mm


; sys_read:
;     syscall_handler 0
;     ;mov rax, 3
;     ;mov r10, rcx
;     ;call syscall

;     ;ret

; sys_write:
;     syscall_handler 1
;     ;mov rax, 4
;     ;call syscall
    
;     ;ret


; sys_clear_screen:
;     syscall_handler 2
;     ;mov rax, 5
;     ;call syscall
  
;     ret

; sys_zoom:
;     ;mov rax, 7
;     ;call syscall
    
;     ret

; sys_time:

;     ;mov rax, 8
;     ;call syscall
  
;     ;ret

; sys_get_regs:

;     ;mov rax, 9
;     ;call syscall

;     ret

; sys_put_rectangle:

;     ;push r10
;     ;mov rax, 10
;     ;mov r10, rcx
;     ;call syscall
;     ;pop r10

;     ret

; sys_get_ticks:

;     ;mov rax, 11
;     ;call syscall
;     ret

; sys_beep:
;     ;mov rax, 14
;     ;call syscall
;     ret

; sys_test_mm:
;     ;mov rax, 12
;     ;call syscall
;     ret