#include <string.h>

#include "int_set.h"

const char emptyState = 'E';
const char tombstoneState = 'T';
const char valueState = 'V';

void initIntSet(IntSet *set) {
  set->count = 0;
  set->capacity = 0;
  set->entries = NULL;
  set->states = NULL;
}

void freeIntSet(IntSet *set) {
  free(set->entries);
  free(set->states);
  initIntSet(set);
}

static size_t findEntry(IntSetValue *entries, char *states, int capacity, IntSetValue value) {
  size_t *tombstone = NULL;
  size_t index = value & (capacity - 1);
  for (;;) {
    char state = states[index];
    if (state == tombstoneState) {
      if (tombstone == NULL) {
        tombstone = &index;
      }
    } else if (state == emptyState) {
      return tombstone != NULL ? *tombstone : index;
    } else if (entries[index] == value) {
      return index;
    }

    index = (index + 1) & (capacity - 1);
  }
}

static void adjustCapacity(IntSet *set, int capacity) {
  IntSetValue *entries = ALLOCATE(IntSetValue, capacity);
  char *states = ALLOCATE(char, capacity);
  memset(states, emptyState, capacity);

  set->count = 0;
  for (int i = 0; i < set->capacity; ++i) {
    if (set->states[i] != valueState) {
      continue;
    }

    IntSetValue entry = set->entries[i];
    size_t index = findEntry(entries, states, capacity, entry);
    entries[index] = entry;
    states[index] = valueState;
    set->count++;
  }

  free(set->entries);
  free(set->states);
  set->entries = entries;
  set->states = states;
  set->capacity = capacity;
}

bool setContainsInt(IntSet *set, IntSetValue value) {
  if (set->count == 0) {
    return false;
  }

  size_t index = findEntry(set->entries, set->states, set->capacity, value);
  return set->states[index] == valueState;
}

bool setAddInt(IntSet *set, IntSetValue value) {
  if (set->count + 1 > set->capacity * SET_MAX_LOAD) {
    int capacity = GROW_CAPACITY(set->capacity);
    adjustCapacity(set, capacity);
  }

  size_t index = findEntry(set->entries, set->states, set->capacity, value);

  bool isNew = set->states[index] == emptyState;
  if (isNew) {
    set->count++;
  }

  set->entries[index] = value;
  set->states[index] = valueState;
  return isNew;
}

bool setDeleteInt(IntSet *set, IntSetValue value) {
  if (set->count == 0) {
    return false;
  }

  size_t index = findEntry(set->entries, set->states, set->capacity, value);
  if (set->states[index] != valueState) {
    return false;
  }

  set->states[index] = tombstoneState;
  return true;
}
