     1                                  [BITS 64]
     2                                  
     3                                  extern floor
     4                                  extern ceil
     5                                  extern pow
     6                                  extern fabs
     7                                  
     8                                  global payload:function
     9                                  global power:function
    10                                  global get_floating_part:function
    11                                  global sigma:function
    12                                  global bbp_getnth_term:function
    13                                  global uint32_swap:function
    14                                  global feisel:function
    15                                  global g_payload_len:data
    16                                  global g_payload_jmp_offset:data
    17                                  global g_payload_start_offset:data
    18                                  
    19 00000000 63040000                g_payload_len dd end - payload
    20 00000004 17040000                g_payload_jmp_offset dd jmp_offset - payload
    21 00000008 F2030000                g_payload_start_offset dd woody_w - payload
    22                                  
    23                                  section .text
    24                                  
    25                                  payload:
    26                                  
    27                                  get_floating_part:
    28                                      ; https://en.wikibooks.org/wiki/X86_Assembly/Floating_Point
    29 0000000C 55                          push rbp
    30 0000000D 4889E5                      mov rbp, rsp
    31 00000010 4883EC10                    sub rsp, 16
    32                                  
    33 00000014 F20F11442408                movsd qword [rsp + 8], xmm0
    34 0000001A 660FEFC0                    pxor xmm0, xmm0
    35 0000001E 660F2F442408                comisd xmm0, [rsp + 8] 
    36 00000024 761E                        jbe g_floor
    37                                  
    38                                    g_ceil:
    39 00000026 F20F10442408                movsd xmm0, qword [rsp + 8]
    40 0000002C E8(00000000)                call ceil
    41 00000031 F20F104C2408                movsd xmm1, qword [rsp + 8]
    42 00000037 F20F5CC8                    subsd xmm1, xmm0
    43 0000003B F20F10C1                    movsd xmm0, xmm1
    44 0000003F 4889EC                      mov rsp, rbp
    45 00000042 5D                          pop rbp
    46 00000043 C3                          ret
    47                                  
    48                                    g_floor:
    49 00000044 F20F10442408                movsd xmm0, qword [rsp + 8]
    50 0000004A E8(00000000)                call floor
    51 0000004F F20F104C2408                movsd xmm1, qword [rsp + 8]
    52 00000055 F20F5CC8                    subsd xmm1, xmm0
    53 00000059 F20F10C1                    movsd xmm0, xmm1
    54 0000005D 4889EC                      mov rsp, rbp
    55 00000060 5D                          pop rbp
    56 00000061 C3                          ret
    57                                  
    58                                  
    59                                  ; int power(long long x, unsigned int y, int p)
    60                                  ; ===> (x ^ y) % p
    61                                  power:
    62 00000062 55                          push rbp
    63 00000063 4889E5                      mov rbp, rsp
    64 00000066 4883EC20                    sub rsp, 32
    65                                  
    66 0000006A 48C744240801000000          mov qword [rsp + 8], 1
    67 00000073 4889542410                  mov qword [rsp + 16], rdx
    68 00000078 48897C2418                  mov qword [rsp + 24], rdi
    69 0000007D 48893424                    mov qword [rsp], rsi
    70                                  
    71 00000081 48833C2400                  cmp qword [rsp], 0
    72 00000086 7476                        je power_0
    73                                  
    74 00000088 BA00000000                  mov rdx, 0
    75 0000008D 488B442418                  mov rax, qword [rsp + 24]
    76 00000092 4C8B4C2410                  mov r9, qword [rsp + 16]
    77 00000097 49F7F1                      div r9
    78 0000009A 4889542418                  mov qword [rsp + 24], rdx
    79                                      
    80 0000009F 48837C241800                cmp qword [rsp + 24], 0
    81 000000A5 7461                        je power_div
    82                                  
    83                                  
    84                                    while_loop:
    85 000000A7 48833C2400                  cmp qword [rsp], 0
    86 000000AC 7E64                        jle power_ret
    87                                  
    88 000000AE 488B0424                    mov rax, qword [rsp]
    89 000000B2 4883E001                    and rax, 1
    90                                      
    91 000000B6 4883F800                    cmp rax, 0
    92 000000BA 741E                        je loop_incs
    93                                  
    94 000000BC 488B442408                  mov rax, qword [rsp + 8]
    95 000000C1 48F7642418                  mul qword [rsp + 24]
    96 000000C6 4889442408                  mov qword [rsp + 8], rax
    97 000000CB BA00000000                  mov rdx, 0
    98                                      ; mov rax, qword [rsp + 8]
    99 000000D0 48F7742410                  div qword [rsp + 16]
   100 000000D5 4889542408                  mov qword [rsp + 8], rdx
   101                                  
   102                                    loop_incs:
   103                                  
   104 000000DA 48D12C24                    shr qword [rsp], 1
   105 000000DE 488B442418                  mov rax, qword [rsp + 24]
   106 000000E3 48F7642418                  mul qword [rsp + 24]
   107 000000E8 4889442418                  mov qword [rsp + 24], rax
   108 000000ED BA00000000                  mov rdx, 0
   109                                      ; mov rax, qword [rsp + 24]
   110 000000F2 48F7742410                  div qword [rsp + 16]
   111 000000F7 4889542418                  mov qword [rsp + 24], rdx
   112                                  
   113 000000FC EBA9                        jmp while_loop
   114                                  
   115                                    power_0:
   116 000000FE B801000000                  mov rax, 1
   117                                  
   118 00000103 4889EC                      mov rsp, rbp
   119 00000106 5D                          pop rbp
   120 00000107 C3                          ret
   121                                  
   122                                    power_div:
   123 00000108 B800000000                  mov rax, 0
   124                                  
   125 0000010D 4889EC                      mov rsp, rbp
   126 00000110 5D                          pop rbp
   127 00000111 C3                          ret
   128                                  
   129                                    power_ret:
   130 00000112 488B442408                  mov rax, qword [rsp + 8]
   131                                  
   132 00000117 4889EC                      mov rsp, rbp
   133 0000011A 5D                          pop rbp
   134 0000011B C3                          ret
   135                                  
   136                                  sigma:
   137 0000011C 55                          push rbp
   138 0000011D 4889E5                      mov rbp, rsp
   139                                  
   140 00000120 4883EC30                    sub rsp, 48
   141                                  
   142 00000124 660FEFC0                    pxor xmm0, xmm0
   143 00000128 F20F110424                  movsd qword [rsp], xmm0 ; res
   144 0000012D F20F100425-                 movsd xmm0, [_1_16]
   144 00000132 [4B040000]         
   145 00000136 F20F11442408                movsd qword [rsp + 8], xmm0; nom
   146 0000013C F20F100425-                 movsd xmm0, [_1]
   146 00000141 [33040000]         
   147 00000145 F20F11442410                movsd qword [rsp + 16], xmm0; add
   148                                  
   149 0000014B 48C744241800000000          mov qword [rsp + 24], 0; k
   150 00000154 48897C2420                  mov qword [rsp + 32], rdi; n
   151 00000159 4889742428                  mov qword [rsp + 40], rsi; a
   152                                  
   153                                    sig_loop_1:
   154 0000015E 488B442420                  mov rax, qword [rsp + 32]
   155 00000163 4839442418                  cmp qword [rsp + 24], rax
   156 00000168 7F65                        jg sig_loop_1_end
   157                                  
   158 0000016A BF10000000                  mov rdi, 16
   159                                  
   160 0000016F 488B742420                  mov rsi, qword [rsp + 32]
   161 00000174 482B742418                  sub rsi, qword [rsp + 24]
   162                                  
   163 00000179 BA00000000                  mov rdx, 0
   164 0000017E B808000000                  mov rax, 8
   165 00000183 4C8B4C2418                  mov r9, qword [rsp + 24]
   166 00000188 49F7E1                      mul r9
   167 0000018B 4889C2                      mov rdx, rax
   168 0000018E 4803542428                  add rdx, qword [rsp + 40]
   169                                  
   170 00000193 F2480F2ACA                  cvtsi2sd xmm1, rdx
   171                                  
   172 00000198 E8C5FEFFFF                  call power
   173                                  
   174                                  
   175 0000019D F2480F2AC0                  cvtsi2sd xmm0, rax
   176                                  
   177 000001A2 F20F5EC1                    divsd xmm0, xmm1
   178                                      
   179                                  
   180 000001A6 F20F11442410                movsd qword [rsp + 16], xmm0; add = ...
   181                                  
   182 000001AC E85BFEFFFF                  call get_floating_part
   183                                  
   184 000001B1 F20F100C24                  movsd xmm1, qword [rsp]
   185 000001B6 F20F58C8                    addsd xmm1, xmm0
   186 000001BA F20F10C1                    movsd xmm0, xmm1
   187 000001BE E849FEFFFF                  call get_floating_part
   188 000001C3 F20F110424                  movsd qword [rsp], xmm0
   189                                  
   190 000001C8 48FF442418                  inc qword [rsp + 24]
   191                                  
   192 000001CD EB8F                        jmp sig_loop_1
   193                                  
   194                                    sig_loop_1_end:
   195                                  
   196 000001CF F20F100425-                 movsd xmm0, [_1]
   196 000001D4 [33040000]         
   197 000001D8 F20F11442410                movsd qword [rsp + 16], xmm0; add = 1
   198                                  
   199 000001DE 48FF442418                  inc qword [rsp + 24]
   200                                  
   201                                    sig_loop_2:
   202 000001E3 F20F10442410                movsd xmm0, qword [rsp + 16] 
   203 000001E9 F20F100C25-                 movsd xmm1, [_0001]
   203 000001EE [23040000]         
   204 000001F2 660F2FC1                    comisd xmm0, xmm1
   205 000001F6 EB63                        jmp sig_loop_2_end
   206                                  
   207 000001F8 F20F100425-                 movsd xmm0, [_16]
   207 000001FD [53040000]         
   208                                      
   209 00000201 488B442420                  mov rax, qword [rsp + 32]
   210 00000206 482B442418                  sub rax, qword [rsp + 24]
   211                                  
   212 0000020B F2480F2AC8                  cvtsi2sd xmm1, rax
   213                                  
   214 00000210 E8(00000000)                call pow
   215                                  
   216 00000215 F20F11442408                movsd qword [rsp + 8], xmm0
   217                                  
   218 0000021B BA00000000                  mov rdx, 0
   219 00000220 B808000000                  mov rax, 8
   220 00000225 4C8B4C2418                  mov r9, qword [rsp + 24]
   221 0000022A 49F7E1                      mul r9
   222 0000022D 4889C2                      mov rdx, rax
   223 00000230 4803542428                  add rdx, qword [rsp + 40]
   224                                  
   225 00000235 F2480F2ACA                  cvtsi2sd xmm1, rdx
   226 0000023A F20F10442408                movsd xmm0, qword [rsp + 8]
   227 00000240 F20F5EC1                    divsd xmm0, xmm1
   228                                  
   229 00000244 F20F11442410                movsd qword [rsp + 16], xmm0
   230                                  
   231 0000024A F20F580424                  addsd xmm0, qword [rsp]
   232                                  
   233 0000024F F20F110424                  movsd qword [rsp], xmm0
   234                                  
   235 00000254 48FF442418                  inc qword [rsp + 24]
   236 00000259 EB88                        jmp sig_loop_2
   237                                    sig_loop_2_end:
   238                                  
   239 0000025B F20F100424                  movsd xmm0, qword [rsp]
   240 00000260 E8A7FDFFFF                  call get_floating_part
   241 00000265 4889EC                      mov rsp, rbp
   242 00000268 5D                          pop rbp
   243 00000269 C3                          ret
   244                                  
   245                                  bbp_getnth_term:
   246 0000026A 55                          push rbp
   247 0000026B 4889E5                      mov rbp, rsp
   248 0000026E 4883EC10                    sub rsp, 16
   249                                  
   250 00000272 48893C24                    mov qword [rsp], rdi; n
   251                                  
   252 00000276 BE01000000                  mov rsi, 1
   253 0000027B E89CFEFFFF                  call sigma
   254                                  
   255 00000280 F20F100C25-                 movsd xmm1, [_4]
   255 00000285 [43040000]         
   256 00000289 F20F59C1                    mulsd xmm0, xmm1
   257                                  
   258 0000028D F20F11442408                movsd qword [rsp + 8], xmm0; res
   259                                  
   260 00000293 488B3C24                    mov rdi, qword [rsp]
   261 00000297 BE04000000                  mov rsi, 4
   262 0000029C E87BFEFFFF                  call sigma
   263                                  
   264 000002A1 F20F100C25-                 movsd xmm1, [_2]
   264 000002A6 [3B040000]         
   265 000002AA F20F59C1                    mulsd xmm0, xmm1
   266                                  
   267 000002AE F20F104C2408                movsd xmm1, qword [rsp + 8]
   268 000002B4 F20F5CC8                    subsd xmm1, xmm0
   269 000002B8 F20F114C2408                movsd qword [rsp + 8], xmm1
   270                                  
   271 000002BE 488B3C24                    mov rdi, qword [rsp]
   272 000002C2 BE05000000                  mov rsi, 5
   273 000002C7 E850FEFFFF                  call sigma
   274                                  
   275 000002CC F20F104C2408                movsd xmm1, qword [rsp + 8]
   276 000002D2 F20F5CC8                    subsd xmm1, xmm0
   277 000002D6 F20F114C2408                movsd qword [rsp + 8], xmm1
   278                                  
   279 000002DC 488B3C24                    mov rdi, qword [rsp]
   280 000002E0 BE06000000                  mov rsi, 6
   281 000002E5 E832FEFFFF                  call sigma
   282                                  
   283 000002EA F20F104C2408                movsd xmm1, qword [rsp + 8]
   284 000002F0 F20F5CC8                    subsd xmm1, xmm0
   285 000002F4 F20F114C2408                movsd qword [rsp + 8], xmm1
   286                                  
   287 000002FA F20F10442408                movsd xmm0, qword [rsp + 8]
   288 00000300 E807FDFFFF                  call get_floating_part
   289                                  
   290 00000305 660FEFC9                    pxor xmm1, xmm1
   291 00000309 660F2FC8                    comisd xmm1, xmm0
   292 0000030D 760D                        jbe bbp_ret
   293                                  
   294 0000030F F20F100C25-                 movsd xmm1, [_1]
   294 00000314 [33040000]         
   295 00000318 F20F58C1                    addsd xmm0, xmm1
   296                                  
   297                                    bbp_ret:
   298                                  
   299 0000031C E8EBFCFFFF                  call get_floating_part
   300 00000321 E8(00000000)                call fabs
   301 00000326 F20F100C25-                 movsd xmm1, [_16]
   301 0000032B [53040000]         
   302 0000032F F20F59C1                    mulsd xmm0, xmm1
   303 00000333 F20F11442408                movsd qword [rsp + 8], xmm0
   304 00000339 E8CEFCFFFF                  call get_floating_part
   305                                  
   306 0000033E F20F104C2408                movsd xmm1, qword [rsp + 8]
   307 00000344 F20F5CC8                    subsd xmm1, xmm0
   308 00000348 F2480F2DC1                  cvtsd2si rax, xmm1
   309                                  
   310 0000034D 4889EC                      mov rsp, rbp
   311 00000350 5D                          pop rbp
   312 00000351 C3                          ret
   313                                  
   314                                  
   315                                  uint32_swap:
   316                                  
   317 00000352 55                        push rbp
   318 00000353 4889E5                    mov rbp, rsp
   319 00000356 4883EC10                  sub rsp, 16
   320                                  
   321 0000035A 8B06                      mov eax, dword [rsi]
   322 0000035C 89442408                  mov dword [rsp + 8], eax
   323                                  
   324 00000360 8B07                      mov eax, dword [rdi]
   325 00000362 8906                      mov dword [rsi], eax
   326 00000364 8B442408                  mov eax, dword [rsp + 8]
   327 00000368 8907                      mov dword [rdi], eax
   328                                  
   329 0000036A 4889EC                    mov rsp, rbp
   330 0000036D 5D                        pop rbp
   331 0000036E C3                        ret
   332                                  
   333                                  feisel:
   334                                    
   335 0000036F 55                        push rbp
   336 00000370 4889E5                    mov rbp, rsp
   337 00000373 4883EC10                  sub rsp, 16
   338                                  
   339 00000377 89F8                      mov eax, edi
   340 00000379 25000000FF                and eax, 0xff000000
   341 0000037E C1E818                    shr eax, 24
   342                                  
   343 00000381 890424                    mov dword [rsp], eax
   344                                  
   345 00000384 89F8                      mov eax, edi
   346 00000386 250000FF00                and eax, 0xff0000
   347 0000038B C1E810                    shr eax, 16
   348                                  
   349 0000038E 89442404                  mov dword [rsp + 4], eax
   350                                  
   351 00000392 89F8                      mov eax, edi
   352 00000394 2500FF0000                and eax, 0xff00
   353 00000399 C1E808                    shr eax, 8
   354                                  
   355 0000039C 89442408                  mov dword [rsp + 8], eax
   356                                  
   357 000003A0 89F8                      mov eax, edi
   358 000003A2 25FF000000                and eax, 0xff
   359                                  
   360 000003A7 8944240C                  mov dword [rsp + 12], eax
   361                                  
   362 000003AB 4831C9                    xor rcx, rcx
   363 000003AE 4D31C0                    xor r8, r8
   364                                  
   365 000003B1 8B0C24                    mov ecx, dword [rsp]
   366 000003B4 4889F0                    mov rax, rsi
   367 000003B7 4801C8                    add rax, rcx
   368                                  
   369 000003BA 448B00                    mov r8d, dword [rax]
   370                                  
   371 000003BD 8B4C2404                  mov ecx, dword [rsp + 4]
   372 000003C1 4889F0                    mov rax, rsi
   373 000003C4 480500010000              add rax, 256
   374 000003CA 4801C8                    add rax, rcx
   375                                  
   376 000003CD 440300                    add r8d, dword [rax]
   377                                    
   378 000003D0 8B4C2408                  mov ecx, dword [rsp + 8]
   379 000003D4 4889F0                    mov rax, rsi
   380 000003D7 480500020000              add rax, 512
   381 000003DD 4801C8                    add rax, rcx
   382                                  
   383 000003E0 443300                    xor r8d, dword [rax]
   384                                  
   385 000003E3 8B4C240C                  mov ecx, dword [rsp + 12]
   386 000003E7 4889F0                    mov rax, rsi
   387 000003EA 480500030000              add rax, 768
   388 000003F0 4801C8                    add rax, rcx
   389                                  
   390 000003F3 440300                    add r8d, dword [rax]
   391                                  
   392 000003F6 4489C0                    mov eax, r8d
   393                                  
   394 000003F9 4889EC                    mov rsp, rbp
   395 000003FC 5D                        pop rbp
   396 000003FD C3                        ret
   397                                  
   398                                  woody_w:
   399 000003FE 50                        push rax
   400 000003FF 57                        push rdi
   401 00000400 56                        push rsi
   402 00000401 52                        push rdx
   403                                  
   404 00000402 BF01000000                mov rdi, 1
   405 00000407 488D3551000000            lea rsi, [rel woody]
   406 0000040E BA10000000                mov rdx, len
   407 00000413 B801000000                mov rax, 0x01
   408 00000418 0F05                      syscall
   409                                  
   410 0000041A 5A                        pop rdx
   411 0000041B 5E                        pop rsi
   412 0000041C 5F                        pop rdi
   413 0000041D 58                        pop rax
   414                                  
   415 0000041E E9(FFFFFFFF)              jmp 0xffffffff
   416                                  jmp_offset:
   417 00000423 2D431CEBE2361A3F          _0001 dq 0.0001
   418 0000042B 0000000000000000          _0 dq 0.0
   419 00000433 000000000000F03F          _1 dq 1.0
   420 0000043B 0000000000000040          _2 dq 2.0
   421 00000443 0000000000001040          _4 dq 4.0
   422 0000044B 000000000000B03F          _1_16 dq 0.0625
   423 00000453 0000000000003040          _16 dq 16.0
   424 0000045B 9A99993E                  tt dd 0.3
   425 0000045F 2E2E2E2E574F4F4459-       woody db "....WOODY.....", 10, 0
   425 00000468 2E2E2E2E2E0A00     
   426                                    len equ $ - woody
   427                                  end:
   428                                  
