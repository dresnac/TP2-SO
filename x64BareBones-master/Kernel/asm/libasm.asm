GLOBAL cpuVendor
GLOBAL getKey
GLOBAL rtc
GLOBAL hlt
GLOBAL inb
GLOBAL outb
GLOBAL timer_tick
GLOBAL acquire
GLOBAL release

GLOBAL Hours
GLOBAL Mins

section .text

acquire:
    mov al, 0
.retry:
    xchg byte [rdi], al
    test al, al
    jnz .retry
    ret

release:
    mov byte [rdi], 1
    ret
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

rtc:
	push rbp
	mov rbp, rsp

	mov al, dil
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

inb:
	push rbp
    mov rbp, rsp

	mov rdx, rdi
	in al, dx

	mov rsp, rbp
	pop rbp
	ret

outb:
	push rbp
    mov rbp, rsp

 	mov rax, rsi
	mov rdx, rdi
	out dx, al

	mov rsp, rbp
	pop rbp
	ret

hlt:
	sti
	hlt
	ret

getKey:
    mov rax, 0
.cicle: 
    in al, 64h
    and al, 0x01
    je .cicle
    in al, 60h

    ret

timer_tick:
	int 0x20
	ret

section .rodata

buffer db "Jorge"
