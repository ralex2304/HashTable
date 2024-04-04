#ifndef HASH_H_
#define HASH_H_

#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef uint32_t Hash_t;

Hash_t hash_always_zero(const char* str);

Hash_t hash_first_letter(const char* str);

Hash_t hash_len(const char* str);

Hash_t hash_sum(const char* str);

Hash_t hash_sum_div_len(const char* str);

Hash_t hash_ror(const char* str);

Hash_t hash_rol(const char* str);


#endif //< #ifndef HASH_H_
