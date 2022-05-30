section .bss
glob resq 8

section .data

section .text

extern printf

extern print_registers

extern scanf

global _start

_start:
	call main
	jmp _end

main:
	push rbp
	mov rbp, rsp
	push QWORD [glob+8]; WPV.G
	push QWORD 4
	pop QWORD [glob+8]; WA.G
	pop rbx
	push QWORD [glob+8]; WPV.G
	pop rax
	mov rsi, rax
	mov rdi, format_int
	mov rax, 0
	call printf

_end_main:
	pop rbp
	ret

_end:
	mov rdi, rax
	mov rax, 60
	syscall
