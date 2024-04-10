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

Hash_t hash_pjw32(const char* str) {
    Hash_t hash = 0;

    Hash_t test = 0;

	for (; *str; str++) {
		hash = (hash << 4) + (unsigned char)(*str);

		if ((test = hash & 0xf0000000) != 0)
			hash = ((hash ^ (test >> 24)) & (0xfffffff));
	}

	return hash;
}

Hash_t hash_jenkins(const char* str) {
    Hash_t hash = 0;

    for (; *str; str++) {
        hash += *str;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

Hash_t hash_fnv(const char* str) {
    Hash_t hash = 0x811c9dc5;

    for (; *str; str++) {
        hash *= 0x01000193;
        hash ^= (Hash_t)*str;
    }

    return hash;
}

Hash_t hash_sedgewick(const char* str) {
    Hash_t hash = 0;

    Hash_t coeff = 63689;

    for (; *str; str++) {
        hash = hash * coeff + *str;
        coeff *= 378551;
    }

    return hash;
}

#define mmix(h,k) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }

Hash_t hash_murmur2A(const char* str) {
	const Hash_t m = 0x5bd1e995;
    Hash_t seed = 0;
	const int r = 24;

	Hash_t h = seed;
    Hash_t k = 0;

    uint32_t len = (uint32_t)strlen(str);

	while (len >= 4) {
		k = *(const unsigned int*)str;

		mmix(h,k);

		str += 4;
		len -= 4;
	}

	unsigned int t = 0;

	switch(len) {
        case 3: t ^= str[2] << 16; [[fallthrough]];
        case 2: t ^= str[1] << 8;  [[fallthrough]];
        case 1: t ^= str[0];
                break;

        default:
            return 0;
	};

	mmix(h, t);
	mmix(h, len);

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}
#undef mmix

Hash_t hash_murmur3(const char* str) {
    const uint32_t len = (uint32_t)strlen(str);

    const uint32_t nblocks = len / 4;

    Hash_t seed = 0;

    Hash_t h1 = seed;

    const Hash_t c1 = 0xcc9e2d51;
    const Hash_t c2 = 0x1b873593;

    //----------
    // body

    const Hash_t* blocks = (const Hash_t*)(str + nblocks * 4);

    for(int i = -nblocks; i; i++) {
        Hash_t k1 = ((const Hash_t*)blocks)[i];

        k1 *= c1;
        k1 = rol_(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = rol_(h1,13);
        h1 = h1*5+0xe6546b64;
    }

    //----------
    // tail

    const uint8_t* tail = (const uint8_t*)(str + nblocks * 4);

    Hash_t k1 = 0;

    switch(len & 3) {
        case 3: k1 ^= tail[2] << 16; [[fallthrough]];
        case 2: k1 ^= tail[1] << 8;  [[fallthrough]];
        case 1: k1 ^= tail[0];
                k1 *= c1; k1 = rol_(k1,15); k1 *= c2; h1 ^= k1;
                break;

        default:
            return 0;
    };

    //----------
    // finalization

    h1 ^= len;

    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;

    return h1;
}
