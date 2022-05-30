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

a:
	push rbp
	mov rbp, rsp
	push QWORD 0
	mov rdi, format_scanf_d
	mov rsi, rsp
	mov rax, 0
	mov rbx, [rsp]
	call scanf
	mov rax, [rsp]
	mov QWORD [rbp - 8], rax
	mov [rsp], rbx
	push QWORD [rbp-8]
	pop rax
	mov rsi, rax
	mov rdi, format_int
	mov rax, 0
	call printf

_end_a:
	pop rbx
	pop rbp
	ret

main:
	push rbp
	mov rbp, rsp
	push QWORD 0
	mov rax, 0
	call a
	push rax

_end_main:
	pop rbx
	pop rbp
	ret

_end:
	mov rdi, rax
	mov rax, 60
	syscall
