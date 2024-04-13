#ifndef CRC_H_
#define CRC_H_

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <immintrin.h>

uint32_t memcrc(const unsigned char* data, size_t size);

inline uint32_t memcrc(const char* data, size_t size) {
    return memcrc((const unsigned char*)data, size);
}

uint32_t strcrc(const char* data);

extern "C" uint32_t asm_calc_crc32_8(const char* str, const size_t len);

inline uint32_t strcrc_asm_8(const char* data, const uint32_t len) {
    assert(data);
    assert(len % 8 == 0);

    uint64_t hash = 0;

    for (size_t i = 0; i < len / 8; i++)
        asm ("crc32 %0, %1\n" : "=r"(hash) : "r"(((const uint64_t*)data)[i]), "r"(hash));

    return (uint32_t)hash;
}

inline uint32_t strcrc_asm(const char* data) {
    assert(data);

    uint32_t hash = 0;

    for (; *data; data++)
        asm ("crc32 %0, %1\n" : "=r"(hash) : "r"(*data), "r"(hash));

    return hash;
}

#endif //< #ifndef CRC_H_
