
%include "rw32-2018.inc"
section .data
    pole db 1,6,3,4,5,2,6
section .text
global CMAIN
CMAIN:
    push ebp
    mov ebp,esp
    
    push 3

    push 7
    push pole
    call lasti
    add esp,12
    
    call WriteInt32
    mov esp, ebp
    pop ebp
    ret 
lasti:
    push ebp
    mov ebp,esp
    push edi
    push ebx
    push ecx
    push esi
    
    mov eax,0
    cmp word[ebp+8],0
    je skip
    cmp dword[ebp+12],0
    jle skip
    
    mov bl,[ebp+16]
    mov al,bl
    mov ecx,[ebp+12]
    mov edi,[ebp+8]
    add edi,ecx
    mov ecx,[ebp+12]
.l:
    cmp al,[edi]
    je .von
    sub edi,1
    dec ecx
    cmp ecx,0
    jne .l
.von:
    mov eax,edi

       
    cmp ecx,0
    jne skip
    
    mov eax,0
    mov bx,[ebp+16]
    mov esi,[ebp+8]
    cmp [esi],bx
    jne skip
    mov eax,[ebp+8]
skip:

    pop esi
    pop ecx
    pop ebx
    pop edi
    mov esp, ebp
    pop ebp
    ret 