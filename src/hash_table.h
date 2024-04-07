#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <assert.h>

#include "config.h"
#include "List/list.h"
#include "hash/crc.h"

#if defined CRC_OPTIMISATION || defined STRCMP_OPTIMISATION
#include <immintrin.h>
#endif

struct HashTable {
    public:

#ifdef HASHES_TEST
        bool ctor(size_t size, Hash_t (*hash_func)(Key_t));
#endif

#ifdef PERF_TEST
        bool ctor(size_t size);
#endif

        void dtor();

        Elem_t* get_elem_by_key(Key_t key) {
            return get_elem_by_key(key, calc_hash(key));
        }

        Elem_t* get_elem_by_key(Key_t key, Hash_t hash);

        inline int insert_elem(Elem_t elem) {
            size_t inserted_index = 0;
            return list_pushback(table_ + (elem.hash % table_size_), elem, &inserted_index);
        };

        inline int insert(Key_t key, Val_t val) {
            Elem_t elem = {.key = key, .val = val, .hash = calc_hash(key)};
            return insert_elem(elem);
        };

#ifdef HASHES_TEST
        inline Hash_t calc_hash(Key_t key) { return hash_func_(key); };
#endif
#ifdef PERF_TEST

#ifndef CRC_OPTIMISATION
        inline Hash_t calc_hash(Key_t key) { return strcrc(key); };
#else //< #ifdef CRC_OPTIMISATION
        inline Hash_t calc_hash(Key_t key) { return asm_calc_crc32(key); };
#endif //< #ifndef CRC_OPTIMISATION

#endif //< #ifdef PERF_TEST

        inline size_t size() { return table_size_; };

        inline List* get_list_by_index(size_t index) {
            assert(index < table_size_);

            return table_ + index;
        };

    private:

        size_t table_size_ = 0;

        List* table_ = nullptr;

#ifdef HASHES_TEST
        Hash_t (*hash_func_)(Key_t) = nullptr;
#endif
};


#endif //< #ifndef HASH_TABLE_H_
