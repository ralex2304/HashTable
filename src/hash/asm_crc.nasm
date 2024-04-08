section .text

global asm_calc_crc32

; uint32_t asm_calc_crc32(const char* str)
asm_calc_crc32:

        movzx   eax, byte [rdi] ; eax = *str - chars
        xor     edx, edx        ; edx = 0      - hash

        ; if (str[0] == 0) return 0;
        test    al, al
        je      .empty

.while_body:
        inc     rdi             ; str++
        crc32   edx, al         ; hash = crc32(hash, *str)

        movzx   eax, byte [rdi] ; eax = *str

        ; if (*str != 0) continue;
        ; else break;
        test    al, al
        jne     .while_body

.empty:
        mov     eax, edx        ; return edx
        ret
