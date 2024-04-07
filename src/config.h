#ifndef HASH_TABLE_CONFIG_H_
#define HASH_TABLE_CONFIG_H_

#include <stdlib.h>

//#define HASHES_TEST
#define PERF_TEST

#define CRC_OPTIMISATION

#define STRCMP_OPTIMISATION


#ifndef NDEBUG

#define INCLUDE_LIST "ListDebug/ListDebug.h"

#else //< #ifdef NDEBUG

#define INCLUDE_LIST "List/List.h"

#endif //< #ifndef NDEBUG

// words in words.txt: 5414
// | load factor | table size |
// |       1     |    5413    |
// |       2     |    2707    |
// |       3     |    1801    |
// |       4     |    1327    |
// |       7     |    773     |
// |       8     |    677     |
// |       9     |    601     |
// |      10     |    541     |
const size_t TABLE_SIZE = 773;

static const size_t HASHES_TEST_NUM = 9;

const size_t LIST_DEFAULT_CAPACITY = 8;

#endif //< #ifndef HASH_TABLE_CONFIG_H_
