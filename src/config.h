#ifndef HASH_TABLE_CONFIG_H_
#define HASH_TABLE_CONFIG_H_

#include <stdlib.h>

#define HASHES_TEST
//#define PERF_TEST

#ifndef NDEBUG

#define INCLUDE_LIST "ListDebug/ListDebug.h"

#else //< #ifdef NDEBUG

#define INCLUDE_LIST "List/List.h"

#endif //< #ifndef NDEBUG

const size_t TABLE_SIZE = 1007;

const size_t LIST_DEFAULT_CAPACITY = 8;

#endif //< #ifndef HASH_TABLE_CONFIG_H_
