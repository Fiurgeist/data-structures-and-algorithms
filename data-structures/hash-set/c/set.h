#ifndef fg_set_h
#define fg_set_h

#include <stdbool.h>
#include <stdint.h>

#include "../../common/c/memory.h"

#define SET_MAX_LOAD 0.75

typedef struct {
  uint32_t *value;
} SetValue;

typedef struct {
  int count;
  int capacity;
  SetValue *entries;
} IntSet;

void initSet(IntSet *set);
void freeSet(IntSet *set);
bool setContains(IntSet *set, uint32_t *value);
bool setAdd(IntSet *set, uint32_t *value);
bool setDelete(IntSet *set, uint32_t *value);

#endif
