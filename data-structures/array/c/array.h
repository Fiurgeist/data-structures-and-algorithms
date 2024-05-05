#ifndef fg_array_h
#define fg_array_h

typedef void* ArrayValue;

typedef struct {
  int capacity;
  int count;
  ArrayValue *values;
} Array;

void initArray(Array *array);
void freeArray(Array *array);
void arrayAdd(Array *array, ArrayValue data);

#endif
