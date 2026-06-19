section .text
global _start

extern main
extern sys_exit

_start:
    ; The kernel pushes argc and argv on the stack before jumping to user mode.
    ; For now, we'll just extract them or assume they are 0.
    
    ; Simple startup: call main(0, NULL)
    xor edi, edi
    xor esi, esi
    call main
    
    ; Pass the return value of main (in eax) to sys_exit
    mov edi, eax
    call sys_exit
    
    ; Should never reach here
.hang:
    jmp .hang
