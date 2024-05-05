#include <assert.h>

#include "set.h"

void testBasic() {
  uint32_t a = 42;
  uint32_t b = 23;

  IntSet set;
  initSet(&set);
  assert(setContains(&set, &a) == false);
  assert(set.capacity == 0);

  setAdd(&set, &a);
  setAdd(&set, &b);

  assert(set.capacity == INITIAL_CAPACITY);
  assert(setContains(&set, &a));
  assert(setContains(&set, &b));

  assert(setDelete(&set, &a));
  assert(setContains(&set, &a) == false);

  freeSet(&set);
}

void testCollisionHandling() {
  IntSet set;
  initSet(&set);

  // test collisions and tombstones by hash being a mutiple of INITIAL_CAPACITY for all entries
  assert(INITIAL_CAPACITY > 4);
  uint32_t a = 0;
  uint32_t b = INITIAL_CAPACITY;
  uint32_t c = INITIAL_CAPACITY * 2;
  uint32_t d = INITIAL_CAPACITY * 3;

  setAdd(&set, &a);
  setAdd(&set, &b);
  setAdd(&set, &c);
  setAdd(&set, &d);

  assert(setDelete(&set, &b));
  assert(setDelete(&set, &c));

  assert(setContains(&set, &a));
  assert(setContains(&set, &d));

  freeSet(&set);
}

int main(int argc, const char *argv[]) {
  testBasic();
  testCollisionHandling();
}
