[BITS 64]

global payload:function
global power:function
global get_floating_part:function
global g_payload_len:data
global g_payload_jmp_offset:data

g_payload_len dd end - payload
g_payload_jmp_offset dd jmp_offset - payload

section .text

payload:


get_floating_part:
  push rbp
  mov rbp, rsp
  mov rdx, 0
  mov rax, rdi
  mov rdi, 1
  div rdi
  mov rax, rdx
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

woody_w:
  push rax
  push rdi
  push rsi
  push rdx

  mov rdi, 1
  lea rsi, [rel woody]
  mov rdx, len
  mov rax, 0x01
  syscall

  pop rdx
  pop rsi
  pop rdi
  pop rax

  jmp 0xffffffff
jmp_offset:
  woody db "....WOODY.....", 10, 0
  len equ $ - woody
end:

