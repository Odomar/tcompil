section .bss
glob resq 0

section .data

section .text

extern printf

extern print_registers

extern scanf

global _start

	format_int: db "%ld", 10, 0
	format_char: db "%c", 10, 0
	format_scanf_c: db "%c", 0
	format_scanf_d: db "%d", 0

tab_var:
	pop rsi
	pop rdx
	mov rbx, rdx
	imul rbx, rax
	mov rdx, rbp
	pop rax
	mov rcx, rax
	add rbx, rcx
	sub rdx, rbx
	push rdx
	push QWORD [rdx]
	push rsi
	ret

tab_glo:
	pop rsi
	pop rdx
	mov rbx, rdx
	imul rbx, rax
	mov rdx, glob
	pop rax
	mov rcx, rax
	add rbx, rcx
	add rdx, rbx
	push rdx
	push QWORD [rdx]
	push rsi
	ret

_start:
	call main
	jmp _end

test:
	push rbp
	mov rbp, rsp
	push QWORD 1
	pop rax
	jmp _end_test

_end_test:
	pop rbp
	ret

main:
	push rbp
	mov rbp, rsp
	push QWORD 0
	push QWORD 0
	push QWORD [rbp-16]
	push QWORD 2
	pop QWORD [rbp-16]
	pop rbx
	push QWORD [rbp-8]
	push QWORD 1
	push QWORD 2
	pop rbx
	pop rax
	add rax, rbx
	push rax
	pop QWORD [rbp-8]
	pop rbx
	push QWORD [rbp-16]
	push QWORD 1
	mov rax, 0
	pop rbx
	sub rax, rbx
	push rax
	pop QWORD [rbp-16]
	pop rbx
	push QWORD [rbp-8]
	pop rax
	mov rsi, rax
	mov rdi, format_int
	mov rax, 0
	call printf
	push QWORD [rbp-16]
	pop rax
	mov rsi, rax
	mov rdi, format_int
	mov rax, 0
	call printf
	push QWORD 1
	pop rax
	mov rsi, rax
	mov rdi, format_int
	mov rax, 0
	call printf
	push QWORD 2
	pop rax
	mov rsi, rax
	mov rdi, format_int
	mov rax, 0
	call printf
	push QWORD 0
	pop rax
	jmp _end_main

_end_main:
	pop rbx
	pop rbx
	pop rbp
	ret

_end:
	mov rdi, rax
	mov rax, 60
	syscall
