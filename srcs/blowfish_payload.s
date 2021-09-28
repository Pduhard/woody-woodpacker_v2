[BITS 64]

global blowfish_payload:function
global power:function
global get_floating_part:function
global sigma:function
global bbp_getnth_term:function
global uint32_swap:function
global feisel:function
global blowfish_decrypt:function
global blowfish_encrypt:function
global blowfish_init:function
global g_blowfish_pld_len:data
global g_blowfish_pld_entry_off:data
global g_blowfish_pld_jmp_off:data
global g_blowfish_pld_checksum_off:data
global g_blowfish_pld_sec_vaddr_off:data
global g_blowfish_pld_sec_size_off:data
global g_blowfish_pld_vaddr_load_off:data

g_blowfish_pld_len dd blowfish_end - blowfish_payload
g_blowfish_pld_entry_off dd blowfish_entry - blowfish_payload
g_blowfish_pld_jmp_off dd blowfish_jmp - blowfish_payload
g_blowfish_pld_checksum_off dd blowfish_checksum - blowfish_payload
g_blowfish_pld_sec_vaddr_off dd blowfish_sec_vaddr - blowfish_payload
g_blowfish_pld_sec_size_off dd blowfish_sec_size - blowfish_payload
g_blowfish_pld_vaddr_load_off dd blowfish_vaddr_load - blowfish_payload

section .text

blowfish_payload:

_strlen:

  push rbp
  mov rbp, rsp
  mov rax, 0

strlen_loop_start:
  cmp byte [rdi + rax], 0
  je strlen_loop_end
  
  inc rax
  jmp strlen_loop_start
  
strlen_loop_end:
  mov rsp, rbp
  pop rbp
  ret

_fabs:
  push rbp
  mov rbp, rsp

  pxor xmm1, xmm1
  comisd xmm0, xmm1
  jnbe _fabs_ret

  movsd xmm1, [rel _m1]
  mulsd xmm0, xmm1

_fabs_ret:
  mov rsp, rbp
  pop rbp
  ret

get_floating_part:
  push rbp
  mov rbp, rsp

  cvtsd2si rax, xmm0
  cvtsi2sd xmm1, rax

  subsd xmm0, xmm1

  pxor xmm1, xmm1
  comisd xmm0, xmm1
  jnbe get_floating_part_end
  addsd xmm0, [rel _1]

get_floating_part_end:
  mov rsp, rbp
  pop rbp
  ret

; int power(long long x, unsigned int y, int p)
; ===> (x ^ y) % p
power:
    push rbp
    mov rbp, rsp
    sub rsp, 32

    mov qword [rsp + 8], 1
    mov qword [rsp + 16], rdx
    mov qword [rsp + 24], rdi
    mov qword [rsp], rsi

    cmp qword [rsp], 0
    je power_0

    mov rdx, 0
    mov rax, qword [rsp + 24]
    mov r9, qword [rsp + 16]
    div r9
    mov qword [rsp + 24], rdx
    
    cmp qword [rsp + 24], 0
    je power_div


  while_loop:
    cmp qword [rsp], 0
    jle power_ret

    mov rax, qword [rsp]
    and rax, 1
    
    cmp rax, 0
    je loop_incs

    mov rax, qword [rsp + 8]
    mul qword [rsp + 24]
    mov qword [rsp + 8], rax
    mov rdx, 0
    ; mov rax, qword [rsp + 8]
    div qword [rsp + 16]
    mov qword [rsp + 8], rdx

  loop_incs:

    shr qword [rsp], 1
    mov rax, qword [rsp + 24]
    mul qword [rsp + 24]
    mov qword [rsp + 24], rax
    mov rdx, 0
    ; mov rax, qword [rsp + 24]
    div qword [rsp + 16]
    mov qword [rsp + 24], rdx

    jmp while_loop

  power_0:
    mov rax, 1

    mov rsp, rbp
    pop rbp
    ret

  power_div:
    mov rax, 0

    mov rsp, rbp
    pop rbp
    ret

  power_ret:
    mov rax, qword [rsp + 8]

    mov rsp, rbp
    pop rbp
    ret

sigma:
    push rbp
    mov rbp, rsp

    sub rsp, 48

    pxor xmm0, xmm0
    movsd qword [rsp], xmm0 ; res
    movsd xmm0, [rel _1_16]
    movsd qword [rsp + 8], xmm0; nom
    movsd xmm0, [rel _1]
    movsd qword [rsp + 16], xmm0; add

    mov qword [rsp + 24], 0; k
    mov qword [rsp + 32], rdi; n
    mov qword [rsp + 40], rsi; a

  sig_loop_1:
    mov rax, qword [rsp + 32]
    cmp qword [rsp + 24], rax
    jg sig_loop_1_end

    mov rdi, 16

    mov rsi, qword [rsp + 32]
    sub rsi, qword [rsp + 24]

    mov rdx, 0
    mov rax, 8
    mov r9, qword [rsp + 24]
    mul r9
    mov rdx, rax
    add rdx, qword [rsp + 40]

    cvtsi2sd xmm1, rdx

    call power


    cvtsi2sd xmm0, rax

    divsd xmm0, xmm1
    

    movsd qword [rsp + 16], xmm0; add = ...

    call get_floating_part

    movsd xmm1, qword [rsp]
    addsd xmm1, xmm0
    movsd xmm0, xmm1
    call get_floating_part
    movsd qword [rsp], xmm0

    inc qword [rsp + 24]

    jmp sig_loop_1

  sig_loop_1_end:

    movsd xmm0, [rel _1]
    movsd qword [rsp + 16], xmm0; add = 1

    inc qword [rsp + 24]

  sig_loop_2:
    movsd xmm0, qword [rsp + 16] 
    movsd xmm1, [rel _0001]
    comisd xmm0, xmm1
    jmp sig_loop_2_end

    movsd xmm0, qword [rsp + 8]
    mulsd xmm0, [rel _1_16]
    movsd qword [rsp + 8], xmm0

    mov rdx, 0
    mov rax, 8
    mov r9, qword [rsp + 24]
    mul r9
    mov rdx, rax
    add rdx, qword [rsp + 40]

    cvtsi2sd xmm1, rdx
    movsd xmm0, qword [rsp + 8]
    divsd xmm0, xmm1

    movsd qword [rsp + 16], xmm0

    addsd xmm0, qword [rsp]

    movsd qword [rsp], xmm0

    inc qword [rsp + 24]
    jmp sig_loop_2
  sig_loop_2_end:

    movsd xmm0, qword [rsp]
    call get_floating_part
    mov rsp, rbp
    pop rbp
    ret

bbp_getnth_term:
    push rbp
    mov rbp, rsp
    sub rsp, 16

    mov qword [rsp], rdi; n

    mov rsi, 1
    call sigma

    movsd xmm1, [rel _4]
    mulsd xmm0, xmm1

    movsd qword [rsp + 8], xmm0; res

    mov rdi, qword [rsp]
    mov rsi, 4
    call sigma

    movsd xmm1, [rel _2]
    mulsd xmm0, xmm1

    movsd xmm1, qword [rsp + 8]
    subsd xmm1, xmm0
    movsd qword [rsp + 8], xmm1

    mov rdi, qword [rsp]
    mov rsi, 5
    call sigma

    movsd xmm1, qword [rsp + 8]
    subsd xmm1, xmm0
    movsd qword [rsp + 8], xmm1

    mov rdi, qword [rsp]
    mov rsi, 6
    call sigma

    movsd xmm1, qword [rsp + 8]
    subsd xmm1, xmm0
    movsd qword [rsp + 8], xmm1

    movsd xmm0, qword [rsp + 8]
    call get_floating_part

    pxor xmm1, xmm1
    comisd xmm1, xmm0
    jbe bbp_ret

    movsd xmm1, [rel _1]
    addsd xmm0, xmm1

  bbp_ret:

    call get_floating_part
    call _fabs
    movsd xmm1, [rel _16]
    mulsd xmm0, xmm1
    movsd qword [rsp + 8], xmm0
    call get_floating_part

    movsd xmm1, qword [rsp + 8]
    subsd xmm1, xmm0
    cvtsd2si rax, xmm1

    mov rsp, rbp
    pop rbp
    ret


uint32_swap:

  push rbp
  mov rbp, rsp
  sub rsp, 16

  mov eax, dword [rsi]
  mov dword [rsp + 8], eax

  mov eax, dword [rdi]
  mov dword [rsi], eax
  mov eax, dword [rsp + 8]
  mov dword [rdi], eax

  mov rsp, rbp
  pop rbp
  ret

feisel:
  
  push rbp
  mov rbp, rsp
  sub rsp, 16

  mov eax, edi
  and eax, 0xff000000
  shr eax, 24

  mov dword [rsp], eax

  mov eax, edi
  and eax, 0xff0000
  shr eax, 16

  mov dword [rsp + 4], eax

  mov eax, edi
  and eax, 0xff00
  shr eax, 8

  mov dword [rsp + 8], eax

  mov eax, edi
  and eax, 0xff

  mov dword [rsp + 12], eax

  xor rcx, rcx
  xor r8, r8

  mov ecx, dword [rsp]
  shl rcx, 2
  mov rax, rsi
  add rax, rcx

  mov r8d, dword [rax]

  mov ecx, dword [rsp + 4]
  shl rcx, 2
  mov rax, rsi
  add rax, 1024
  add rax, rcx

  add r8d, dword [rax]
  
  mov ecx, dword [rsp + 8]
  shl rcx, 2
  mov rax, rsi
  add rax, 2048
  add rax, rcx

  xor r8d, dword [rax]

  mov ecx, dword [rsp + 12]
  shl rcx, 2
  mov rax, rsi
  add rax, 3072
  add rax, rcx

  add r8d, dword [rax]

  mov eax, r8d

  mov rsp, rbp
  pop rbp
  ret

blowfish_decrypt:
  push rbp
  mov rbp, rsp
  sub rsp, 32

  mov rax, rdi
  shr rax, 32
  mov dword [rsp], eax
  mov dword [rsp + 4], edi

  mov qword [rsp + 8], 17
  mov qword [rsp + 16], rsi
  mov qword [rsp + 24], rdx

blowfish_decrypt_loop:
  cmp qword [rsp + 8], 1
  jle blowfish_decrypt_end_loop

  mov rax, qword [rsp + 8]
  shl rax, 2
  add rax, qword [rsp + 16]
  mov eax, dword [rax]
  xor eax, dword [rsp]
  mov dword [rsp], eax

  mov edi, dword [rsp]
  mov rsi, qword [rsp + 24]
  call feisel
  xor eax, dword [rsp + 4]
  mov dword [rsp + 4], eax

  mov rdi, rsp
  mov rsi, rsp
  add rsi, 4
  call uint32_swap

  dec qword [rsp + 8]
  jmp blowfish_decrypt_loop

blowfish_decrypt_end_loop:
  mov rdi, rsp
  mov rsi, rsp
  add rsi, 4
  call uint32_swap
  
  mov rax, qword [rsp + 16]
  mov eax, dword [rax]
  xor eax, dword [rsp]
  mov dword [rsp], eax

  mov rax, qword [rsp + 16]
  add rax, 4
  mov eax, dword [rax]
  xor eax, dword [rsp + 4]
  mov dword [rsp + 4], eax

  mov eax, dword [rsp]
  shl rax, 32
  xor rdi, rdi
  mov edi, dword [rsp + 4] 
  or rax, rdi
  mov rsp, rbp
  pop rbp
  ret

blowfish_encrypt:
  push rbp
  mov rbp, rsp
  sub rsp, 32

  mov rax, rdi
  shr rax, 32
  mov dword [rsp], eax
  mov dword [rsp + 4], edi

  mov qword [rsp + 8], 0
  mov qword [rsp + 16], rsi
  mov qword [rsp + 24], rdx

blowfish_encrypt_loop:
  cmp qword [rsp + 8], 16
  jge blowfish_encrypt_end_loop

  mov rax, qword [rsp + 8]
  shl rax, 2
  add rax, qword [rsp + 16]
  mov eax, dword [rax]
  xor eax, dword [rsp]
  mov dword [rsp], eax

  mov edi, dword [rsp]
  mov rsi, qword [rsp + 24]
  call feisel
  xor eax, dword [rsp + 4]
  mov dword [rsp + 4], eax

  mov rdi, rsp
  mov rsi, rsp
  add rsi, 4
  call uint32_swap

  inc qword [rsp + 8]
  jmp blowfish_encrypt_loop

blowfish_encrypt_end_loop:
  mov rdi, rsp
  mov rsi, rsp
  add rsi, 4
  call uint32_swap
  
  mov rax, qword [rsp + 16]
  add rax, 68
  mov eax, dword [rax]
  xor eax, dword [rsp]
  mov dword [rsp], eax

  mov rax, qword [rsp + 16]
  add rax, 64
  mov eax, dword [rax]
  xor eax, dword [rsp + 4]
  mov dword [rsp + 4], eax

  mov eax, dword [rsp]
  shl rax, 32
  xor rdi, rdi
  mov edi, dword [rsp + 4] 
  or rax, rdi
  mov rsp, rbp
  pop rbp
  ret

blowfish_init:
  push rbp
  mov rbp, rsp
  
  

  mov rax, 80
  add rax, qword [rel _pi_cap]
  sub rsp, rax

  mov qword [rsp], rdi ; key
  mov qword [rsp + 8], rsi ; p
  mov qword [rsp + 16], rdx ; s



  cmp qword [rsp], 0x0
  je no_strlen

  call _strlen
  mov qword [rsp + 24], rax ; key len 

no_strlen:
  ; mov rax, qword [rsp + 8]
  ; mov qword [rsp + 30], rax ; p_fill
  ; mov rax, qword [rsp + 8]
  ; mov qword [rsp + 30], rax ; s_fill

  mov qword [rsp + 32], 0 ; i = 0
  mov qword [rsp + 40], 0 ; j = 0
  mov qword [rsp + 48], 0 ; n = 0
  mov qword [rsp + 56], 0 ; init_val = 0

  
bbp_init_loop_start:
  mov rax, [rel _pi_cap]
  cmp qword [rsp + 32], rax
  jge bbp_init_loop_end

  mov rdi, qword [rsp + 32]
  call bbp_getnth_term

  mov rdi, rsp
  add rdi, 64
  add rdi, qword [rsp + 32]
  mov byte [rdi], al

  ; xor rsi, rsi
  ; mov sil, byte [rdi]
  ; lea rdi, [p_format]
  ; call printf

  inc qword [rsp + 32]
  jmp bbp_init_loop_start
bbp_init_loop_end:



  mov qword [rsp + 32], 0

bbp_p_fill_loop_start:
  cmp qword [rsp + 32], 72
  jge bbp_p_fill_loop_end

  mov rax, qword [rsp + 48]
  add rax, rsp
  add rax, 64

  xor rcx, rcx
  mov cl, byte [rax]
  shl cl, 4
  add rax, 1
  or cl, byte [rax]

  mov r8, qword [rsp + 8]
  add r8, qword [rsp + 32]
  mov byte [r8], cl

  cmp qword [rsp], 0x0
  je p_fill_loop_no_key

  mov rdx, 0
  mov rax, qword [rsp + 32]
  mov rdi, qword [rsp + 24]
  div rdi
  mov rax, qword [rsp]
  add rax, rdx
  mov al, byte [rax]

  xor byte [r8], al

p_fill_loop_no_key:

  add qword [rsp + 48], 2

  mov rdx, 0
  mov rax, qword [rsp + 48]
  mov rdi, [rel _pi_cap]
  div rdi
  mov qword [rsp + 48], rdx

  inc qword [rsp + 32]
  jmp bbp_p_fill_loop_start
bbp_p_fill_loop_end:

  ; mov rsp, rbp
  ; pop rbp
  ; ret

  mov qword [rsp + 32], 0

bbp_s_fill_loop_start:
  cmp qword [rsp + 32], 4096
  jge bbp_s_fill_loop_end

  mov rax, qword [rsp + 48]
  add rax, rsp
  add rax, 64

  xor rcx, rcx
  mov cl, byte [rax]
  shl cl, 4
  add rax, 1
  or cl, byte [rax]

  mov rax, qword [rsp + 16]
  add rax, qword [rsp + 32]
  mov byte [rax], cl

  add qword [rsp + 48], 2

  mov rdx, 0
  mov rax, qword [rsp + 48]
  mov rdi, [rel _pi_cap]
  div rdi
  mov qword [rsp + 48], rdx

  inc qword [rsp + 32]
  jmp bbp_s_fill_loop_start

bbp_s_fill_loop_end:


  mov qword [rsp + 32], 0

bbp_p_cross_loop_start:
  cmp qword [rsp + 32], 18
  jge bbp_p_cross_loop_end

  mov rdi, qword [rsp + 56]
  mov rsi, qword [rsp + 8]
  mov rdx, qword [rsp + 16]

  call blowfish_encrypt
  mov qword [rsp + 56], rax
  mov rax, qword [rsp + 32]
  shl rax, 2
  add rax, qword  [rsp + 8]
  mov rdi, qword [rsp + 56]
  shr rdi, 32
  mov dword [rax], edi

  add rax, 4
  mov rdi, qword [rsp + 56]
  mov dword [rax], edi

  add qword [rsp + 32], 2
  jmp bbp_p_cross_loop_start
bbp_p_cross_loop_end:

  mov qword [rsp + 32], 0

bbp_s_cross_loop_start:
  cmp qword [rsp + 32], 1024
  jge bbp_s_cross_loop_end

  mov rdi, qword [rsp + 56]
  mov rsi, qword [rsp + 8]
  mov rdx, qword [rsp + 16]

  call blowfish_encrypt
  mov qword [rsp + 56], rax
  mov rax, qword [rsp + 32]
  shl rax, 2
  add rax, qword  [rsp + 16]
  mov rdi, qword [rsp + 56]
  shr rdi, 32
  mov dword [rax], edi

  add rax, 4
  mov rdi, qword [rsp + 56]
  mov dword [rax], edi

  add qword [rsp + 32], 2
  jmp bbp_s_cross_loop_start
bbp_s_cross_loop_end:
  ; jl wrong_key

  ; mov 



  mov rsp, rbp
  pop rbp
  ret

blowfish_entry:
  push rbp
  mov rbp, rsp
  push rax
  push rdi
  push rsi
  push rdx

  sub rsp, 4256

  cmp dword [rel checksum], 0
  je blowfish_no_key

  mov rdi, 1
  lea rsi, [rel key_ask]
  mov rdx, len_key_ask
  mov rax, 0x01 ; write
  syscall

  mov rdi, 0
  mov rsi, rsp
  mov rdx, 64
  mov rax, 0x0 ; read
  syscall

  add rax, rsp
  sub rax, 1 ; '\n' replace by '\0'
  mov byte [rax], 0

  mov rdi, rsp
  mov rsi, rsp
  add rsi, 64
  mov rdx, rsp
  add rdx, 136

woody_call_blowfish_init:
  call blowfish_init_rip ; //blowfish init

  mov rdi, qword [rsp]
  mov rsi, rsp
  add rsi, 64
  mov rdx, rsp
  add rdx, 136

woody_call_blowfish_encrypt:
  call blowfish_encrypt_rip ; //blowfish_encrypt
  cmp rax, qword [rel checksum]
  je nok_end

  mov rdi, 1
  lea rsi, [rel nok]
  mov rdx, len_nok
  mov rax, 0x01
  syscall

  mov rdi, 1
  mov rax, 60 ; exit(EXIT_FAILURE)
  syscall

nok_end:
  jmp woody_decrypt

blowfish_no_key:
  mov rdi, 0x0
  mov rsi, rsp
  add rsi, 64
  mov rdx, rsp
  add rdx, 136

woody_call_blowfish_init_no_key:
  call blowfish_init_no_key_rip ; //blowfish init

woody_decrypt:

  mov qword [rsp + 4240], 0
  lea rax, [rel sec_vaddr]
blowfish_vaddr_load:
  mov qword [rsp + 4248], rax

woody_decrypt_loop:
  mov rax, [rel sec_size]
  cmp qword [rsp + 4240], rax
  jge woody_decrypt_loop_end

  mov rax, qword [rsp + 4248]
  mov rdi, qword [rax]

  mov rsi, rsp
  add rsi, 64

  mov rdx, rsp
  add rdx, 136

woody_call_blowfish_decrypt:
  call blowfish_decrypt_rip

  mov rdi, qword [rsp + 4248]
  mov qword [rdi], rax
  
  add qword [rsp + 4240], 8
  add qword [rsp + 4248], 8
  jmp woody_decrypt_loop
woody_decrypt_loop_end:

  mov rdi, 1
  lea rsi, [rel woody]
  mov rdx, len
  mov rax, 0x01
  syscall

  add rsp, 4256
  pop rdx
  pop rsi
  pop rdi
  pop rax

  mov rsp, rbp
  pop rbp
  jmp 0xffffffff
blowfish_jmp:

  _0001 dq 0.0001
  _0 dq 0.0
  _1 dq 1.0
  _2 dq 2.0
  _4 dq 4.0
  _1_16 dq 0.0625
  _16 dq 16.0
  _m1 dq -1.0
  tt dd 0.3
  woody db "....WOODY.....", 10, 0
  len equ $ - woody
  _pi_cap dq 128
blowfish_sec_vaddr:
  sec_vaddr dq 0
blowfish_sec_size:
  sec_size dq 0
blowfish_checksum:
  checksum dq 0
  key_ask db "password: ", 0
  len_key_ask equ $ - key_ask
  ok db "OK!", 10, 0
  len_ok equ $ - ok
  nok db "Wrong password", 10, 0
  len_nok equ $ - nok
  blowfish_decrypt_rip equ blowfish_decrypt - woody_call_blowfish_decrypt - 1
  blowfish_encrypt_rip equ blowfish_encrypt - woody_call_blowfish_encrypt - 1
  blowfish_init_rip equ blowfish_init - woody_call_blowfish_init - 1
  blowfish_init_no_key_rip equ blowfish_init - woody_call_blowfish_init_no_key - 1
blowfish_end: