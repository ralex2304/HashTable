section .text

global asm_calc_crc32

asm_calc_crc32:

        movzx   eax, byte [rdi]
        xor     edx, edx

        test    al, al
        je      .empty

.while_body:
        inc     rdi
        crc32   edx, al

        movzx   eax, byte [rdi]

        test    al, al
        jne     .while_body

.empty:
        mov     eax, edx
        ret
