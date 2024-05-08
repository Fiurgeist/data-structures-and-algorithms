#ifndef fg_set_h
#define fg_set_h

#include <stdbool.h>
#include <stdint.h>

#include "../../common/c/memory.h"

#define SET_MAX_LOAD 0.75

typedef struct {
  uint32_t *value;
} PtrSetValue;

typedef struct {
  int count;
  int capacity;
  PtrSetValue *entries;
} PtrSet;

void initSet(PtrSet *set);
void freeSet(PtrSet *set);
bool setContains(PtrSet *set, uint32_t *value);
bool setAdd(PtrSet *set, uint32_t *value);
bool setDelete(PtrSet *set, uint32_t *value);

#endif
