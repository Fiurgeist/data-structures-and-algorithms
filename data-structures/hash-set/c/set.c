#include "set.h"

#define QNAN                ((uint64_t)0x7ffc000000000000)
#define TOMBSTONE_VAL       ((uint32_t*)(uintptr_t)(QNAN | 1))
#define IS_TOMBSTONE(value) (value == TOMBSTONE_VAL)
// #define TOMBSTONE_VAL       ((uint64_t)(QNAN | 1))
// #define IS_TOMBSTONE(value) ((uint64_t)(uintptr_t)(value) == TOMBSTONE_VAL)

void initSet(PtrSet *set) {
  set->count = 0;
  set->capacity = 0;
  set->entries = NULL;
}

void freeSet(PtrSet *set) {
  free(set->entries);
  initSet(set);
}

static PtrSetValue* findEntry(PtrSetValue *values, int capacity, uint32_t value) {
  PtrSetValue *tombstone = NULL;
  size_t index = value & (capacity - 1);
  for (;;) {
    PtrSetValue *entry = &values[index];
    if (entry->value == NULL) {
      return tombstone != NULL ? tombstone : entry;
    } else if (IS_TOMBSTONE(entry->value)) {
      if (tombstone == NULL) {
        tombstone = entry;
      }
    } else if (*entry->value == value) {
      return entry;
    }

    index = (index + 1) & (capacity - 1);
  }
}

static void adjustCapacity(PtrSet *set, int capacity) {
  PtrSetValue *entries = ALLOCATE(PtrSetValue, capacity);
  for (int i = 0; i < capacity; i++) {
    entries[i].value = NULL;
  }

  set->count = 0;
  for (int i = 0; i < set->capacity; ++i) {
    PtrSetValue *entry = &set->entries[i];
    if (entry->value == NULL || IS_TOMBSTONE(entry->value)) {
      continue;
    }

    PtrSetValue *dest = findEntry(entries, capacity, *entry->value);
    dest->value = entry->value;
    set->count++;
  }

  free(set->entries);
  set->entries = entries;
  set->capacity = capacity;
}

bool setContains(PtrSet *set, uint32_t *value) {
  if (set->count == 0) {
    return false;
  }

  PtrSetValue *entry = findEntry(set->entries, set->capacity, *value);
  if (entry->value == NULL || IS_TOMBSTONE(entry->value)) {
    return false;
  }

  return true;
}

bool setAdd(PtrSet *set, uint32_t *value) {
  if (set->count + 1 > set->capacity * SET_MAX_LOAD) {
    int capacity = GROW_CAPACITY(set->capacity);
    adjustCapacity(set, capacity);
  }

  PtrSetValue *entry = findEntry(set->entries, set->capacity, *value);

  bool isNew = entry->value == NULL;
  if (isNew) {
    set->count++;
  }

  entry->value = value;
  return isNew;
}

bool setDelete(PtrSet *set, uint32_t *value) {
  if (set->count == 0) {
    return false;
  }

  PtrSetValue *entry = findEntry(set->entries, set->capacity, *value);
  if (entry->value == NULL) {
    return false;
  }

  // entry->value = (uint32_t*)(uintptr_t)TOMBSTONE_VAL;
  entry->value = TOMBSTONE_VAL;
  return true;
}
