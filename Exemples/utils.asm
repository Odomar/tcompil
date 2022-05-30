extern printf

section .data
	format_registers db "rax:%ld rbx:%ld rcx:%ld rdx:%ld", 10, 0
	format_stacks db "haut de la pile (rsp): 0x%x, bas de la pile (rbp): 0x%x", 10, 0

section .text

global print_stacks		; declaration de la fonction 
global print_registers	; declatation de la fonction

print_registers:
	push rbp
	mov rbp, rsp
	
	push rax
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	
	mov r8, rdx 
	mov rcx, rcx
	mov rdx, rbx
    mov rsi, rax
    mov rdi, format_registers
    mov rax, 0
    call printf WRT ..plt	
	
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rax
	
	pop rbp
	ret	

print_stacks:
	push rbp
	mov rbp, rsp

	push rax
	push rdx
	push rdi
	push rsi

	mov rdx, [rsp+32]
	
	mov rax, rsp
	add rax, 48
    mov rsi, rax
     
    mov rdi, format_stacks
    mov rax, 0
    call printf WRT ..plt
    
	pop rsi
	pop rdi
	pop rdx
	pop rax
	
	pop rbp
	ret	
