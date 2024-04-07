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

    char* words = nullptr;

    long size = 0;
    STATUS_CHECK_RAISE(file_open_read_close("test/test_input/words.txt", &words, &size, 1 + 32),
                                                                                         // 32 for AVX
                                                                                    FREE(words));

#ifdef HASHES_TEST
    STATUS_CHECK_RAISE(hashes_test_(words),                                         FREE(words));
#endif

#ifdef PERF_TEST
    STATUS_CHECK_RAISE(perf_test_(words),                                           FREE(words));
#endif

    FREE(words);

    return 0;
}

inline static bool insert_word_(HashTable* table, Key_t word) {
    Hash_t hash = table->calc_hash(word);

    Elem_t* elem = table->get_elem_by_key(word, hash);

    if (elem == nullptr) {

        if (table->insert(word, strlen(word)) != List::OK)
            return false;

    }

    return true;
}

#ifdef HASHES_TEST

static Status::Statuses hashes_test_(char* words) {
    assert(words);

    HashTable tables[HASHES_TEST_NUM] = {};

    tables[0].ctor(TABLE_SIZE, hash_always_zero);
    tables[1].ctor(TABLE_SIZE, hash_first_letter);
    tables[2].ctor(TABLE_SIZE, hash_len);
    tables[3].ctor(TABLE_SIZE, hash_sum);
    tables[4].ctor(100,        hash_sum);
    tables[5].ctor(TABLE_SIZE, hash_sum_div_len);
    tables[6].ctor(TABLE_SIZE, hash_ror);
    tables[7].ctor(TABLE_SIZE, hash_rol);
    tables[8].ctor(TABLE_SIZE, strcrc);

    char* word = strtok(words, "\n");

    while (word != nullptr && *word != '\0') {

        for (size_t i = 0; i < HASHES_TEST_NUM; i++) {

            if (!insert_word_(tables + i, word)) {
                for (size_t j = 0; j < HASHES_TEST_NUM; j++)
                    tables[i].dtor();

                return Status::LIST_ERROR;
            }
        }

        word = strtok(nullptr, "\n");
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

    char* keys = nullptr;

    long size = 0;
    STATUS_CHECK(file_open_read_close("test/test_input/keys.txt", &keys, &size, 1 + 32)); //< 32 for AVX

    char* key = strtok(keys, "\n");

    size_t found = 0;

    while (key != nullptr && *key != '\0') {

        Elem_t* elem = table->get_elem_by_key(key);

        if (elem != nullptr)
            found++;

        key = strtok(nullptr, "\n");
    }

    printf("%zu\n", found);

    FREE(keys);

    return Status::NORMAL_WORK;
}

static Status::Statuses perf_test_(char* words) {

    HashTable table = {};
    if (!table.ctor(TABLE_SIZE))
        return Status::MEMORY_EXCEED;

    char* word = strtok(words, "\n");

    while (word != nullptr && *word != '\0') {

        for (size_t i = 0; i < HASHES_TEST_NUM; i++) {

            if (!insert_word_(&table, word)) {

                table.dtor();
                return Status::LIST_ERROR;
            }
        }

        word = strtok(nullptr, "\n");
    }

    Status::Statuses result = perf_test_read_keys_and_search_(&table);

    table.dtor();

    return result;
}
#endif //< #ifdef PERF_TEST
