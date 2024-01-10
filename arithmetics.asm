
global _bigadd
global _seed256
global _aes_prng
global _store_le
global _load_le
global _prime_prng_fix
global _bigmul
global _bigcmp
global _bigmod

global __bigmul2
global __bigdiv2

section .text
    _load_le:
    _store_le:
    ; rcx - dst
    ; rdx - src
    ; r8 - byte size divisible by 8

    ;Clobbers rax, r9
        xor r9, r9
    _store_le_cycle:
        cmp r8, r9
        jz _store_le_cycle_end
        mov rax, [rdx + r9]
        bswap rax
        mov [rcx + r9], rax
        add r9, 8
        jmp _store_le_cycle
    _store_le_cycle_end:
        ret

    _seed256:
    ; rcx - buffer to store 256 bits of randomness from the CPU

    ;Clobbers rax
        rdseed rax
        mov [rcx], rax
        rdseed rax
        mov [rcx + 8], rax
        rdseed rax
        mov [rcx + 16], rax
        rdseed rax
        mov [rcx + 24], rax
        ret

    _aes_prng:
    ; rcx - state/seed
    ; rdx - ptr to store random
    ; r8 - bytes len, divisibly by 16
        movups xmm1, [rcx]
        pxor xmm0, xmm0
    _aes_prng_cycle:
        test r8, r8
        jz _aes_prng_end
        sub r8, 16
        aesenc xmm0, xmm1
        pxor  xmm0, xmm1
        pxor  xmm1, xmm0
        pxor  xmm0, xmm1
        movups [rdx + r8], xmm0
        jmp _aes_prng_cycle
    _aes_prng_end:
        movups [rcx], xmm1
        ret

    _prime_prng_fix:
    ;rcx - buffer
    ;rdx - size
        or byte[rcx], 0x80
        sub rdx, 1
        or byte [rcx + rdx], 1
        ret

    _bigadd:
    ;rcx - dst
    ;rdx - op1
    ;r8 - op2
    ;r9 - bytes amount to add up, divisible by 8

    ;Clobbers rax, r9, r11
        clc
        xor r11, r11
    _bigadd_cycle:
        test r9, r9
        jz _bigadd_end
        sub r9, 8
        mov rax, [rdx + r9]
        test r11b, r11b
        jz _bigadd_nc
        stc
    _bigadd_nc:
        adc rax, [r8 + r9]
        setc r11b
        mov [rcx + r9], rax
        jmp _bigadd_cycle
    _bigadd_end:
        xor rax, rax
        mov al, r11b
        ret

    _bigmul:
    ;rcx - dst
    ;rdx - op1
    ;r8 - op2
    ;r9 - size of the buffer. Operands are exprected to be the same size

    ;Clobber rax, rdx, r8, r9, r10, r11
        push rbp
        push rsi
        push rdi
        push rbx
        mov rbp, rsp
        sub rsp, 24

        mov [rsp], rcx ; dst
        mov [rsp + 8], rdx ; op1
        mov rsi, r8 ; op2
        mov [rsp + 16], r9 ; sz
        mov rdx, rcx
        xor r10, r10 ; counter of bytes
    _bigmul_outer_cycle:
        xor rdi, rdi ; counter of bits
        cmp r10, [rsp + 16]
        mov rbx, [rsi + r10]
        jz _bigmul_end
    _bigmul_inner_cycle:
        cmp rdi, 64
        jz _bigmul_inner_end
        mov r8, [rsp]
        mov r9, [rsp + 16]
        call _bigadd
        shl rbx, 1
        jc _bigmul_add_case
_bigmul_inner_continue:
        inc rdi
        jmp _bigmul_inner_cycle
    _bigmul_add_case:
        mov r8, [rsp + 8]
        mov r9, [rsp + 16]
        call _bigadd
        jmp _bigmul_inner_continue
    _bigmul_inner_end:
        add r10, 8
        jmp _bigmul_outer_cycle

    _bigmul_end:
        add rsp, 24
        pop rbx
        pop rdi
        pop rsi
        pop rbp
        ret


    _bigcmp:
    ;rcx - op1
    ;rdx - op2
    ;r8 - sz in bytes, divisible by 8

    ;Clobbers rax, r9
    ;out - eax. 0 - op1, 1 - op2, 2 - eq
        xor r9, r9
    _bigcmp_cycle:
        cmp r9, r8
        jz _bigcmp_eq_end
        mov rax, [rcx + r9]
        add r9, 8
        cmp rax, [rdx + r9 - 8]
        je _bigcmp_cycle
        ja _bigcmp_g_end
        mov eax, 1
        jmp _bigcmp_end 
    _bigcmp_eq_end:
        mov eax, 2
        jmp _bigcmp_end
    _bigcmp_g_end:
        mov eax, 0
    _bigcmp_end:
        ret

    __bigmul2:
    ;rcx - op1/dst
    ;r8 - sz

    ;Clobbers rax, r9, r10,r11
        xor r11, r11
        xor r10, r10
        ; mov r9, rdx
        mov r9, r8
    __bigmul2_cycle:
        test r9, r9
        jz __bigmul2_end
        sub r9, 8
        mov rax, [rcx + r9]
        shl rax, 1
        setc r10b
        or al, r11b
        mov r11b, r10b
        mov [rcx + r9], rax
        jmp __bigmul2_cycle
    __bigmul2_end:
        ret

    __bigdiv2:
    ;rcx - op1/dst
    ;r8 - sz

    ;Clobbers rax, r9, r10,r11
        xor r11, r11
        xor r10, r10
        xor r9, r9
    __bigdiv2_cycle:
        cmp r9, r8
        ; cmp r9, rdx
        jz __bigdiv2_end
        mov rax, [rcx + r9]
        shr rax, 1
        setc r10b
        shl r11, 63
        or rax, r11
        xor r11, r11
        mov r11b, r10b
        mov [rcx + r9], rax
        add r9, 8
        jmp __bigdiv2_cycle
    __bigdiv2_end:
        ret

    __bigneg:
    ;rcx - op1
    ;r8 - sz

    ;Clobbers rax, r9, r11
        mov r9, r8
        xor r11, r11
        mov r11b, 1
    __bigneg_cycle:
        test r9, r9
        jz __bigneg_end
        sub r9, 8
        mov rax, [rcx + r9]
        not rax
        add rax, r11
        setc r11b
        mov [rcx + r9], rax
        jmp __bigneg_cycle
    __bigneg_end:
        xor rax, rax
        ret

    __bigpos:
    ;rcx - op1
    ;r8 - sz

    ;Clobbers rax, r9, r10, r11
        mov r9, r8
        or r10, -1
        xor r11, r11
    __bigpos_cycle:
        test r9, r9
        jz __bigpos_end
        sub r9, 8
        mov rax, [rcx + r9]
        test r11b, r11b
        jz __bigpos_nc
        stc
    __bigpos_nc:
        adc rax, r10
        setc r11b
        not rax
        mov [rcx + r9], rax
        jmp __bigpos_cycle
    __bigpos_end:
        xor rax, rax
        ret


_bigmod:
    ; op1 % op2
    ;rcx - op1
    ;rdx - op2
    ;r8 - sz in bytes, divisible by 8

    ;Clobbers all scratch (rax, rcx, rdx, r8, r9, r10, r11)
        push rbp
        push rsi
        push rdi
        mov rbp, rsp
        sub rsp, r8
        sub rsp, 24
        mov [rsp], r8; sz
        mov [rsp + 8], rcx ; op1
        mov [rsp + 16], rdx ; op2
        ;[rsp + 24] - sz sized buffer for M
        ;rbx - sz sized buffer for M * 2
        jmp _bigmod_init_cycle
    _bigmod_cycle:
        mov r8, [rsp]
    ;check a < b if yes, jump to end 
        mov rcx, [rsp + 8]
        mov rdx, [rsp + 16]
    _bigmod_init_cycle:
        call _bigcmp
        cmp eax, 1
        jz _bigmod_end
    ;populate storage for m (rsp + 24)
        mov rsi, rdx
        lea rdi, [rsp + 24]
        mov rcx, r8
        shr rcx, 3
        rep movsq
    ;mul2 storage for m*2
        lea rcx, [rsp + 24]
        mov rdx, [rsp + 8]
    _bigmod_m2mul:
        call __bigmul2
    ;compare m*2 with a, if bigger work with m

        call _bigcmp
        test eax, eax
        jz _bigmod_sub
        jmp _bigmod_m2mul
    ;substraction part
    _bigmod_sub:
    ;convert m to negative and add to a
        ; lea rcx, [rsp + 24]
        call __bigdiv2
        call __bigneg
        mov rcx, [rsp + 8]
        mov rdx, rcx
        mov r9, r8
        lea r8, [rsp + 24]
        call _bigadd
        jmp _bigmod_cycle
    _bigmod_end:
        pop rax
        lea rsp, [rsp + rax + 16]
        pop rdi
        pop rsi
        pop rbp
        ret
