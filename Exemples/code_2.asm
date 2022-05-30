section .bss
glob resq 0

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
	push QWORD 0
	push QWORD [rbp-8]
	push QWORD 4
	pop QWORD [rbp-8]
	pop rbx
	push QWORD [rbp-8]
	pop rax
	mov rsi, rax
	mov rdi, format_int
	mov rax, 0
	call printf

_end_main:
	pop rbx
	pop rbp
	ret

_end:
	mov rdi, rax
	mov rax, 60
	syscall
