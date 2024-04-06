#include "hash_table.h"

#ifdef HASHES_TEST
bool HashTable::ctor(size_t size, Hash_t (*hash_func)(Key_t)) {
    assert(hash_func);
#endif
#ifdef PERF_TEST
bool HashTable::ctor(size_t size) {
#endif
    assert(size > 0);

    table_ = (List*)calloc(size, sizeof(List));

    if (table_ == nullptr)
        return false;

    for (size_t i = 0; i < size; i++) {
        table_[i] = {};

        if (LIST_CTOR_CAP(table_ + i, LIST_DEFAULT_CAPACITY) != List::OK) {

            FREE(table_);
            return false;
        }
    }

    table_size_ = size;

#ifdef HASHES_TEST
    hash_func_ = hash_func;
#endif

    return true;
}

void HashTable::dtor() {
    for (size_t i = 0; i < table_size_; i++)
        list_dtor(table_ + i);

    FREE(table_);

    table_size_ = 0;
}

Elem_t* HashTable::get_elem_by_key(Key_t key, Hash_t hash) {
    assert(table_);

    List* list = table_ + (hash % table_size_);

    for (size_t list_node = list_head(list); list_node > 0; list_node = list->arr[list_node].next) {
        Elem_t elem = list->arr[list_node].elem;

        if (elem.hash == hash && strcmp(key, elem.key) == 0) {
            return &list->arr[list_node].elem;
        }
    }

    return nullptr;
}
