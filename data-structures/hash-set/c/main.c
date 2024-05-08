#include <assert.h>

#include "set.h"
#include "int_set.h"

void testBasic() {
  uint32_t a = 42;
  uint32_t b = 23;

  PtrSet set;
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
  PtrSet set;
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

void testIncrease() {
  PtrSet set;
  initSet(&set);

  assert(INITIAL_CAPACITY <= 8);
  uint32_t a = 0;
  uint32_t b = INITIAL_CAPACITY;
  uint32_t c = INITIAL_CAPACITY * 2;
  uint32_t d = INITIAL_CAPACITY * 3;
  uint32_t e = INITIAL_CAPACITY * 4;
  uint32_t f = INITIAL_CAPACITY * 5;
  uint32_t g = INITIAL_CAPACITY * 6;
  uint32_t h = INITIAL_CAPACITY * 7;
  uint32_t i = INITIAL_CAPACITY * 8;

  setAdd(&set, &a);
  setAdd(&set, &b);
  setAdd(&set, &c);
  setAdd(&set, &d);
  setAdd(&set, &e);
  setAdd(&set, &f);
  setAdd(&set, &g);
  setAdd(&set, &h);
  setAdd(&set, &i);

  assert(setContains(&set, &a));
  assert(setContains(&set, &b));
  assert(setContains(&set, &c));
  assert(setContains(&set, &d));
  assert(setContains(&set, &e));
  assert(setContains(&set, &f));
  assert(setContains(&set, &g));
  assert(setContains(&set, &h));
  assert(setContains(&set, &i));

  freeSet(&set);
}

void testBasicInt() {
  IntSet set;
  initIntSet(&set);
  assert(setContainsInt(&set, 42) == false);
  assert(set.capacity == 0);

  setAddInt(&set, 42);
  setAddInt(&set, 23);

  assert(set.capacity == INITIAL_CAPACITY);
  assert(setContainsInt(&set, 42));
  assert(setContainsInt(&set, 23));

  assert(setDeleteInt(&set, 42));
  assert(setContainsInt(&set, 42) == false);

  freeIntSet(&set);
}

void testCollisionHandlingInt() {
  IntSet set;
  initIntSet(&set);

  // test collisions and tombstones by hash being a mutiple of INITIAL_CAPACITY for all entries
  assert(INITIAL_CAPACITY > 4);
  uint32_t a = 0;
  uint32_t b = INITIAL_CAPACITY;
  uint32_t c = INITIAL_CAPACITY * 2;
  uint32_t d = INITIAL_CAPACITY * 3;

  setAddInt(&set, a);
  setAddInt(&set, b);
  setAddInt(&set, c);
  setAddInt(&set, d);

  assert(setDeleteInt(&set, b));
  assert(setDeleteInt(&set, c));

  assert(setContainsInt(&set, a));
  assert(setContainsInt(&set, d));

  freeIntSet(&set);
}

void testIncreaseInt() {
  IntSet set;
  initIntSet(&set);

  assert(INITIAL_CAPACITY <= 8);
  uint32_t a = 0;
  uint32_t b = INITIAL_CAPACITY;
  uint32_t c = INITIAL_CAPACITY * 2;
  uint32_t d = INITIAL_CAPACITY * 3;
  uint32_t e = INITIAL_CAPACITY * 4;
  uint32_t f = INITIAL_CAPACITY * 5;
  uint32_t g = INITIAL_CAPACITY * 6;
  uint32_t h = INITIAL_CAPACITY * 7;
  uint32_t i = INITIAL_CAPACITY * 8;

  setAddInt(&set, a);
  setAddInt(&set, b);
  setAddInt(&set, c);
  setAddInt(&set, d);
  setAddInt(&set, e);
  setAddInt(&set, f);
  setAddInt(&set, g);
  setAddInt(&set, h);
  setAddInt(&set, i);

  assert(setContainsInt(&set, a));
  assert(setContainsInt(&set, b));
  assert(setContainsInt(&set, c));
  assert(setContainsInt(&set, d));
  assert(setContainsInt(&set, e));
  assert(setContainsInt(&set, f));
  assert(setContainsInt(&set, g));
  assert(setContainsInt(&set, h));
  assert(setContainsInt(&set, i));

  freeIntSet(&set);
}

int main(int argc, const char *argv[]) {
  testBasic();
  testCollisionHandling();
  testIncrease();

  testBasicInt();
  testCollisionHandlingInt();
  testIncreaseInt();
}
