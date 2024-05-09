#ifndef fg_array_h
#define fg_array_h

typedef void* ArrayValue;

typedef struct {
  size_t capacity;
  size_t count;
  ArrayValue *values;
} Array;

void initArray(Array *array);
void freeArray(Array *array);
void arrayAdd(Array *array, ArrayValue data);

#endif
