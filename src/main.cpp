#include <stdlib.h>

#include "config.h"

#include "List/log/log.h"
#include "List/list.h"
#include "utils/statuses.h"
#include "file/file.h"
#include "hash_table.h"
#include "hash/crc.h"

LogFileData log_file = {"log"};


#ifdef HASHES_TEST
static Status::Statuses hashes_test_(char* words);

static Status::Statuses hashes_test_print_results_(HashTable* tables);
#endif

#ifdef PERF_TEST
static Status::Statuses perf_test_read_keys_and_search_(HashTable* table);

static Status::Statuses perf_test_(char* words);
#endif

int main() {

    MappedFile words = {};

    STATUS_CHECK_RAISE(words.open_and_map("test/test_input/words_bin.txt"),     words.unmap_and_close());

#ifdef HASHES_TEST
    STATUS_CHECK_RAISE(hashes_test_(words.buf),                                 words.unmap_and_close());
#endif

#ifdef PERF_TEST
    STATUS_CHECK_RAISE(perf_test_(words.buf),                                   words.unmap_and_close());
#endif

    words.unmap_and_close();

    return Status::OK_EXIT;
}

inline static bool insert_word_(HashTable* table, Key_t word) {
    Hash_t hash = table->calc_hash(word);

    Elem_t* elem = table->get_elem_by_key(word, hash);

    if (elem == nullptr) {

        if (table->insert(word, 1) != List::OK)
            return false;

    }

    return true;
}

#ifdef HASHES_TEST

static Status::Statuses hashes_test_(char* words) {
    assert(words);

    Hash_t (*hash_funcs[HASHES_TEST_NUM])(Key_t) = {
        hash_always_zero, hash_first_letter, hash_len, hash_sum, hash_sum, hash_sum_div_len,
        hash_ror, hash_rol, strcrc, hash_pjw32, hash_jenkins, hash_fnv, hash_sedgewick,
        hash_murmur2A, hash_murmur3
    };

    HashTable tables[HASHES_TEST_NUM] = {};

    for (size_t i = 0; i < HASHES_TEST_NUM; i++)
        tables[i].ctor(i == 4 ? 100 : TABLE_SIZE, hash_funcs[i]);

    for (char* word = words; *word != '\0'; word += AVX_BLOCK_SIZE) {

        for (size_t i = 0; i < HASHES_TEST_NUM; i++) {

            if (!insert_word_(tables + i, word)) {
                for (size_t j = 0; j < HASHES_TEST_NUM; j++)
                    tables[i].dtor();

                return Status::LIST_ERROR;
            }
        }
    }

    Status::Statuses result = hashes_test_print_results_(tables);

    for (size_t i = 0; i < HASHES_TEST_NUM; i++)
        tables[i].dtor();

    return result;
}

static Status::Statuses hashes_test_print_results_(HashTable* tables) {
    assert(tables);

    for (size_t i = 0; i < HASHES_TEST_NUM; i++) {

        printf("%zu\n", tables[i].size());

        for (size_t cell = 0; cell < tables[i].size(); cell++)
            printf("%zd\n", tables[i].get_list_by_index(cell)->size);
    }

    return Status::NORMAL_WORK;
}
#endif //< #ifdef HASHES_TEST

#ifdef PERF_TEST
static Status::Statuses perf_test_read_keys_and_search_(HashTable* table) {
    assert(table);

    MappedFile keys = {};

    STATUS_CHECK(keys.open_and_map("test/test_input/keys_bin.txt"));

    size_t found = 0;

    for (char* key = keys.buf; *key != '\0'; key += AVX_BLOCK_SIZE) {

        Elem_t* elem = table->get_elem_by_key(key);

        if (elem != nullptr)
            found++;
    }

    printf("%zu\n", found);

    STATUS_CHECK(keys.unmap_and_close());

    return Status::NORMAL_WORK;
}

static Status::Statuses perf_test_(char* words) {

    HashTable table = {};
    if (!table.ctor(TABLE_SIZE))
        return Status::MEMORY_EXCEED;

    for (char* word = words; *word != '\0'; word += AVX_BLOCK_SIZE) {

        if (!insert_word_(&table, word)) {

            table.dtor();
            return Status::LIST_ERROR;
        }
    }

    Status::Statuses result = perf_test_read_keys_and_search_(&table);

    table.dtor();

    return result;
}
#endif //< #ifdef PERF_TEST
