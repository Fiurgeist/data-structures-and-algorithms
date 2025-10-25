#include <assert.h>
#include <stdio.h>

#include "gap_buffer.h"

int main(int argc, const char *argv[]) {
  GapBuffer buffer;
  initGapBuffer(&buffer);

  char a = 'a';
  char b = 'b';
  char c = 'c';
  printf("a: %p\n", &a);
  printf("b: %p\n", &b);
  printf("c: %p\n", &c);

  gapBufferInsert(&buffer, &a, 0);
  printf("start: %d; end %d\n", buffer.gapStart, buffer.gapEnd);
  assert(buffer.gapStart == 1);
  assert(buffer.gapEnd == 7);

  gapBufferInsert(&buffer, &b, 1);
  printf("start: %d; end %d\n", buffer.gapStart, buffer.gapEnd);
  assert(buffer.gapStart == 2);
  assert(buffer.gapEnd == 7);

  gapBufferInsert(&buffer, &c, 2);

  for (size_t i = 0; i < buffer.capacity; ++i) {
    printf("%d - %p\n", i, (char*)buffer.values[i]);
  }
  assert((char*)buffer.values[0] == &a);
  assert((char*)buffer.values[1] == &b);


  char x = 'x';
  printf("x: %p\n", &x);
  gapBufferInsert(&buffer, &x, 1);
  for (size_t i = 0; i < 8; ++i) {
    printf("%d - %p\n", i, (char*)buffer.values[i]);
  }
  printf("start: %d; end %d\n", buffer.gapStart, buffer.gapEnd);
  assert(buffer.gapStart == 2);
  assert(buffer.gapEnd == 5);
  assert((char*)buffer.values[0] == &a);
  assert((char*)buffer.values[1] == &x);
  assert((char*)buffer.values[6] == &b);
  assert((char*)buffer.values[7] == &c);

  char y = 'y';
  printf("y: %p\n", &y);
  gapBufferInsert(&buffer, &y, 3);
  for (size_t i = 0; i < buffer.capacity; ++i) {
    printf("%d - %p\n", i, (char*)buffer.values[i]);
  }
  printf("start: %d; end %d\n", buffer.gapStart, buffer.gapEnd);
  assert(buffer.gapStart == 4);
  assert(buffer.gapEnd == 6);
  assert((char*)buffer.values[0] == &a);
  assert((char*)buffer.values[1] == &x);
  assert((char*)buffer.values[2] == &b);
  assert((char*)buffer.values[3] == &y);
  assert((char*)buffer.values[7] == &c);

  printf("retrieve:\n");
  for (size_t i = 0; i < buffer.count; ++i) {
    printf("%d - %c\n", i, *(char*)gapBufferRetrieve(&buffer, i));
  }

  assert(buffer.count == 5);
  gapBufferRemove(&buffer, 4);
  assert((char*)buffer.values[0] == &a);
  assert((char*)buffer.values[1] == &x);
  assert((char*)buffer.values[2] == &b);
  assert((char*)buffer.values[3] == &y);
  printf("start: %d; end %d\n", buffer.gapStart, buffer.gapEnd);
  assert(buffer.count == 4);
  assert(buffer.gapStart == 4);
  assert(buffer.gapEnd == 7);

  gapBufferRemove(&buffer, 1);
  assert((char*)buffer.values[0] == &a);
  assert((char*)buffer.values[6] == &b);
  assert((char*)buffer.values[7] == &y);
  printf("start: %d; end %d\n", buffer.gapStart, buffer.gapEnd);
  assert(buffer.count == 3);
  assert(buffer.gapStart == 1);
  assert(buffer.gapEnd == 5);

  freeGapBuffer(&buffer);
}
