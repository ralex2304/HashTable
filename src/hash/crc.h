#ifndef CRC_H_
#define CRC_H_

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

uint32_t memcrc(const unsigned char* data, size_t size);

inline uint32_t memcrc(const char* data, size_t size) {
    return memcrc((const unsigned char*)data, size);
}

uint32_t strcrc(const char* data);

extern "C" uint32_t asm_calc_crc32(const char* str);

#endif //< #ifndef CRC_H_
