; asm/vec_math.asm
; SPDX-License-Identifier: GPL-3.0-or-later
; Copyright (c) 2025 Bharath

global vec_add_asm
global vec_sub_asm
global vec_mul_asm
global vec_div_asm

section .text

; MACRO: IMPL_VEC_OP
; Generates a complete function for a specific SIMD instruction.
; %1 = Function Name (e.g., vec_add_asm)
; %2 = Packed Instruction (e.g., addpd)
; %3 = Scalar Instruction (e.g., addsd)

%macro IMPL_VEC_OP 3
%1:
    ; Arguments: RDI=count, RSI=a, RDX=b, RCX=out
    
    cmp rdi, 2
    jl .scalar_loop_%1

.simd_loop_%1:
    movupd xmm0, [rsi]    ; Load 2 doubles from A
    movupd xmm1, [rdx]    ; Load 2 doubles from B
    
    %2 xmm0, xmm1         ; PERFORM THE MATH (Packed)

    movupd [rcx], xmm0    ; Store result
    
    add rsi, 16
    add rdx, 16
    add rcx, 16
    sub rdi, 2
    cmp rdi, 2
    jge .simd_loop_%1
    
    cmp rdi, 0
    je .done_%1

.scalar_loop_%1:
    movsd xmm0, [rsi]     ; Load 1 double
    %3 xmm0, [rdx]        ; PERFORM THE MATH (Scalar)
    movsd [rcx], xmm0
    
    add rsi, 8
    add rdx, 8
    add rcx, 8
    dec rdi
    jnz .scalar_loop_%1

.done_%1:
    ret
%endmacro


; Instantiate the functions using the macro

; 1. Addition (+)
IMPL_VEC_OP vec_add_asm, addpd, addsd

; 2. Subtraction (-)
IMPL_VEC_OP vec_sub_asm, subpd, subsd

; 3. Multiplication (*)
IMPL_VEC_OP vec_mul_asm, mulpd, mulsd

; 4. Division (/)
IMPL_VEC_OP vec_div_asm, divpd, divsd

; Security: Mark stack as non-executable
section .note.GNU-stack noalloc noexec nowrite progbits