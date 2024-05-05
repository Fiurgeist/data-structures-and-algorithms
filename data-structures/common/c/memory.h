#ifndef fg_memory_h
#define fg_memory_h

#include <stdlib.h>

#define INITIAL_CAPACITY 8

#define GROW_CAPACITY(capacity) ((capacity) < INITIAL_CAPACITY ? INITIAL_CAPACITY : (capacity) * 2)

#define ALLOCATE(type, count) (type*)malloc(sizeof(type) * (count))

#define GROW_ARRAY(type, pointer, newCount) (type*)realloc(pointer, sizeof(type) * (newCount))

#endif
