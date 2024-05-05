#include <assert.h>
#include <stdio.h>

#include "array.h"

typedef struct {
  int foo;
} Value;

int main(int argc, const char *argv[]) {
  Array array;
  initArray(&array);

  Value a = { .foo = 42 };
  Value b = { .foo = 23 };

  arrayAdd(&array, &a);
  arrayAdd(&array, &b);

  assert(((Value*)array.values[0])->foo == 42);
  assert(((Value*)array.values[1])->foo == 23);

  freeArray(&array);
}
