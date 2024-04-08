section .text

global asm_calc_crc32

; uint32_t asm_calc_crc32(const char* str)
asm_calc_crc32:

        movzx   edx, byte [rdi] ; edx = *str - chars
        xor     eax, eax        ; eax = 0    - hash

        ; if (str[0] == 0) return 0;
        test    dl, dl
        je      .empty

.while_body:
        inc     rdi             ; str++
        crc32   eax, dl         ; hash = crc32(hash, *str)

        movzx   edx, byte [rdi] ; edx = *str

        ; if (*str != 0) continue;
        ; else break;
        test    dl, dl
        jne     .while_body

.empty:
        ret
