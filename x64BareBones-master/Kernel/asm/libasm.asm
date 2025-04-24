GLOBAL cpuVendor
GLOBAL getKey
GLOBAL rtc
GLOBAL hlt
GLOBAL inb
GLOBAL outb

GLOBAL Hours
GLOBAL Mins

section .text
	
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

section .rodata

buffer db "Jorge"
