#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <assert.h>

#include "config.h"

#include "List/list.h"

struct HashTable {
    public:

#ifdef HASHES_TEST
        bool ctor(size_t size, Hash_t (*hash_func)(Key_t));
#endif

#ifdef PERF_TEST
        bool ctor(size_t size);
#endif

        void dtor();

        inline Elem_t* get_elem_by_key(Key_t key) {
            return get_elem_by_key(key, calc_hash_(key));
        };

        Elem_t* get_elem_by_key(Key_t key, Hash_t hash);

        inline int insert_elem(Elem_t elem);

    private:

        size_t table_size_ = 0;

        List* table_ = nullptr;

        inline Hash_t calc_hash_(Key_t key);

#ifdef HASHES_TEST
        Hash_t (*hash_func_)(Key_t);
#endif
};


#endif //< #ifndef HASH_TABLE_H_
