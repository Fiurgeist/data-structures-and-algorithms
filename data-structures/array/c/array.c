#include <stddef.h>

#include "../../common/c/memory.h"
#include "array.h"

void initArray(Array *array) {
  array->capacity = 0;
  array->count = 0;
  array->values = NULL;
}

void freeArray(Array *array) {
  free(array->values);
  initArray(array);
}

void arrayAdd(Array *array, ArrayValue value) {
  if (array->count >= array->capacity) {
    array->values = GROW_ARRAY(ArrayValue, array->values, array->capacity);
    array->capacity = GROW_CAPACITY(array->capacity);
  }

  array->values[array->count++] = value;
}
