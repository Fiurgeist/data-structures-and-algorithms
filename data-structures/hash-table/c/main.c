#include <assert.h>

#include "table.h"

#define AS_VALUE(value) ((void*)value)
#define FROM_VALUE(value) (*(int*)&value)

// FNV-1a algorithm, see http://www.isthe.com/chongo/tech/comp/fnv/#FNV-param
const uint32_t fnvPrime = 16777619;
const uint32_t offsetBasis = 2166136261;

static uint32_t hashString(const char *key, int length) {
  uint32_t hash = offsetBasis;
  for (int i = 0; i < length; ++i) {
    hash ^= (uint8_t)key[i];
    hash *= fnvPrime;
  }

  return hash;
}

static String makeString(char *chars, int length) {
  String string;
  string.length = length;
  string.chars = chars;
  string.hash = hashString(chars, length);;

  return string;
}

void testBasic() {
  Table table;
  initTable(&table);

  String key1 = makeString("foo", 3);
  String key2 = makeString("bar", 3);

  TableValue ret;

  assert(tableGet(&table, &key1, &ret) == false);
  assert(table.capacity == 0);

  tableSet(&table, &key1, AS_VALUE(42));
  tableSet(&table, &key2, AS_VALUE(23));

  assert(table.capacity == INITIAL_CAPACITY);
  assert(tableGet(&table, &key1, &ret));
  assert(FROM_VALUE(ret) == 42);
  assert(tableGet(&table, &key2, &ret));
  assert(FROM_VALUE(ret) == 23);

  assert(tableDelete(&table, &key1));
  assert(tableGet(&table, &key1, &ret) == false);

  freeTable(&table);
}

void testCollisionHandling() {
  Table table;
  initTable(&table);

  // test collisions and tombstones by hash being a mutiple of INITIAL_CAPACITY for all entries
  assert(INITIAL_CAPACITY > 4);
  String key1 = {.chars = "foo",  .length = 3, .hash = 0};
  String key2 = {.chars = "fo2",  .length = 3, .hash = INITIAL_CAPACITY};
  String key3 = {.chars = "fo3",  .length = 3, .hash = INITIAL_CAPACITY * 2};
  String key4 = {.chars = "fo4",  .length = 3, .hash = INITIAL_CAPACITY * 3};

  TableValue ret;

  tableSet(&table, &key1, AS_VALUE(42));
  tableSet(&table, &key2, AS_VALUE(23));
  tableSet(&table, &key3, AS_VALUE(5));
  tableSet(&table, &key4, AS_VALUE(3));

  assert(tableDelete(&table, &key2));
  assert(tableDelete(&table, &key3));

  assert(tableGet(&table, &key4, &ret));
  assert(FROM_VALUE(ret) == 3);

  freeTable(&table);
}

int main(int argc, const char *argv[]) {
  testBasic();
  testCollisionHandling();
}
