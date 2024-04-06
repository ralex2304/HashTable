#include "hash.h"

inline static Hash_t ror_(Hash_t x, unsigned char shift) {
    return (x >> shift) + (x << (sizeof(Hash_t) * 8 - shift));
}

inline static Hash_t rol_(Hash_t x, unsigned char shift) {
    return (x << shift) + (x >> (sizeof(Hash_t) * 8 - shift));
}


Hash_t hash_always_zero(const char* str) {
    (void) str;
    return 0;
}

Hash_t hash_first_letter(const char* str) {
    return *str;
}

Hash_t hash_len(const char* str) {
    return (Hash_t)strlen(str);
}

Hash_t hash_sum(const char* str) {
    Hash_t res = 0;

    while (*str)
        res += *(str++);

    return res;
}

Hash_t hash_sum_div_len(const char* str) {
    return hash_sum(str) / hash_len(str);
}

Hash_t hash_ror(const char* str) {
    Hash_t res = 0;

    while (*str)
        res += ror_(res, 1) ^ *(str++);

    return res;
}

Hash_t hash_rol(const char* str) {
    Hash_t res = 0;

    while (*str)
        res += rol_(res, 1) ^ *(str++);

    return res;
}
