section .text

global asm_calc_crc32_8

; uint32_t asm_calc_crc32_8(const char* str, const size_t len)
asm_calc_crc32_8:

        xor eax, eax                    ; eax  = 0 - hash
        lea rcx, [rdi + rsi]

        ; for (; str != str_ + len; str += 8)
.for_body:
        mov rdx, qword [rdi]            ; rdx = ((uint64_t*)str)[i]

        add rdi, 8                      ; i += 8
        crc32 rax, rdx                  ; hash = crc32(hash, ((uint64_t*)str)[i])

        ; if (i < len) continue;
        ; else break;
        cmp rdi, rcx
        jne .for_body

        ret
