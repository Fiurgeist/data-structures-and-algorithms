#ifndef fg_int_set_h
#define fg_int_set_h

#include <stdbool.h>
#include <stdint.h>

#include "../../common/c/memory.h"

#define SET_MAX_LOAD 0.75

typedef uint32_t IntSetValue;

typedef struct {
  size_t count;
  size_t capacity;
  IntSetValue *entries;
  char *states;
} IntSet;

void initIntSet(IntSet *set);
void freeIntSet(IntSet *set);
bool setContainsInt(IntSet *set, IntSetValue value);
bool setAddInt(IntSet *set, IntSetValue value);
bool setDeleteInt(IntSet *set, IntSetValue value);

#endif
