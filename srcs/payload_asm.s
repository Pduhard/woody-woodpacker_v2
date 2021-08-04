[BITS 64]

global payload:function
global g_payload_len:data
global g_payload_jmp_offset:data

g_payload_len dd end - payload
g_payload_jmp_offset dd jmp_offset - payload

section .text

payload:
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

