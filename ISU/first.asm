%include "rw32-2018.inc"

section .data
    ; zde budou vase data
    pA dd 1,2,3,4,5,6,7,7,9
    pB dd 1,2,3,4,5,6,7,8,9

    
section .text
_main:
    mov esi, pA
    mov edi, pB
    mov ecx, 9
_loop:
dec ecx
mov ebx, [esi]
cmp ebx, dword[edi]
jne end_f
add esi, 4
add edi, 4
cmp ecx, 0
jg _loop
jmp end_t

end_f:
    mov eax, 0
    ret
    
end_t:
    mov eax, 1
        ret