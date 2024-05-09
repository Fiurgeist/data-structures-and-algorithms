#ifndef fg_table_h
#define fg_table_h

#include <stdbool.h>
#include <stdint.h>

#include "../../common/c/memory.h"

#define TABLE_MAX_LOAD 0.75

typedef struct {
  uint32_t length;
  char *chars;
  uint32_t hash;
} String;

typedef void* TableValue;

typedef struct {
  String *key;
  TableValue value;
} Entry;

typedef struct {
  size_t count;
  size_t capacity;
  Entry *entries;
} Table;

void initTable(Table *table);
void freeTable(Table *table);
bool tableGet(Table *table, String *key, TableValue *value);
bool tableSet(Table *table, String *key, TableValue value);
bool tableDelete(Table *table, String *key);
void tableAddAll(Table *from, Table *to);

#endif
