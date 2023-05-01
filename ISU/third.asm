%include "rw32-2018.inc"
CEXTERN  malloc

section .data
    ; zde budou vase data

section .text
_main:
    mov ecx, 1
    cmp ecx, 0
    jle _ret_f
    pusha
    push ecx
    mov edx, ecx
    add edx, edx
    push edx
    call malloc
    pop edx
    pop ecx
    push eax
    mov edx, 0
_loop:
    cmp edx, 3
    jl _set_one
    mov ebx, [eax - 4]
    add ebx, [eax - 6]
    mov [eax], ebx
    
_loop_ret:
    add eax, 2
    inc edx
    cmp edx, ecx   
    jl _loop 
    popa    
    pop eax
    ret
    
_set_one:
    mov [eax], word 1
    jmp _loop_ret
_ret_f:
    mov eax, 0
    ret