[BITS 64]

global xor_payload:function

global g_xor_pld_len:data
global g_xor_pld_entry_off:data
global g_xor_pld_jmp_off:data
global g_xor_pld_sec_vaddr_off:data
global g_xor_pld_sec_size_off:data
; global g_xor_pld_checksum_off:data


g_xor_pld_len dd xor_end - xor_payload
g_xor_pld_entry_off dd xor_entry - xor_payload
g_xor_pld_jmp_off dd xor_jmp_off - xor_payload
g_xor_pld_sec_vaddr_off dd xor_sec_vaddr_off - xor_payload
g_xor_pld_sec_size_off dd xor_sec_size_off - xor_payload

section .text

xor_payload:

xor_entry:
    ; pushf
    ; push rbp
    ; mov rbp, rsp

    push rax
    push rdi
    push rsi
    ; push rsp
    push rdx
    push rcx

    mov al, 0x9b
    mov rdi, [rel sec_size]
    mov rcx, 0

xor_decrypt_loop_start:
    cmp rcx, rdi
    jge xor_decrypt_loop_end

    mov rsi, [rel sec_vaddr] , leaaaaaaaaaaaaaaaaaaaaaa
    add rsi, rcx

    mov dl, byte [rsi]
    xor dl, al
    mov byte [rsi], dl

    inc rcx
    jmp xor_decrypt_loop_start
xor_decrypt_loop_end:

    mov edi, 1
    lea rsi, [rel woody]
    mov rdx, len
    mov rax, 0x01
    ; mov byte [$-1200], 4
    syscall

    pop rcx
    pop rdx
    ; pop rsp
    pop rsi
    pop rdi
    pop rax
    mov rsp, rbp
    pop rbp
    ; popf

    jmp 0xffffffff
xor_jmp_off:
    woody db "....WOODY.....", 10, 0
    len equ $ - woody
xor_sec_vaddr_off:
    sec_vaddr dq 0
xor_sec_size_off:
    sec_size dq 0
xor_end:

