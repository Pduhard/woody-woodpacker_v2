[BITS 64]

global xor_payload:function

global g_xor_pld_len:data
global g_xor_pld_entry_off:data
global g_xor_pld_jmp_off:data
global g_xor_pld_sec_vaddr_off:data
global g_xor_pld_sec_size_off:data
global g_xor_pld_vaddr_load_off:data


g_xor_pld_len dd xor_end - xor_payload
g_xor_pld_entry_off dd xor_entry - xor_payload
g_xor_pld_jmp_off dd xor_jmp - xor_payload
g_xor_pld_sec_vaddr_off dd xor_sec_vaddr - xor_payload
g_xor_pld_sec_size_off dd xor_sec_size - xor_payload
g_xor_pld_vaddr_load_off dd xor_vaddr_load - xor_payload
section .text

xor_payload:

xor_entry:
    pushfq
    push rax
    push rdi
    push rsi
    push rdx
    push rcx

    mov al, 0x9b
    mov rdi, [rel sec_size]

    lea rsi, [rel sec_vaddr]
xor_vaddr_load:

    mov rcx, 0

xor_decrypt_loop_start:
    cmp rcx, rdi
    jge xor_decrypt_loop_end

    mov dl, byte [rsi]
    xor dl, al
    mov byte [rsi], dl

    inc rcx
    inc rsi
    jmp xor_decrypt_loop_start
xor_decrypt_loop_end:

    mov edi, 1
    lea rsi, [rel woody]
    mov rdx, len
    mov rax, 0x01
    syscall

    pop rcx
    pop rdx
    pop rsi
    pop rdi
    pop rax
    popfq

    jmp 0xffffffff
xor_jmp:
    woody db "....WOODY.....", 10, 0
    len equ $ - woody
xor_sec_vaddr:
    sec_vaddr dq 0
xor_sec_size:
    sec_size dq 0
xor_end:

