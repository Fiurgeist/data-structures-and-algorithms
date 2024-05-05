#include <assert.h>
#include <stddef.h>

#include "linked_list.h"

int main(int argc, const char *argv[]) {
  LinkedList list = {.head = NULL};

  int middle = 23;
  llPushFront(&list, &middle);
  assert(list.head->data == &middle);
  assert(list.head->next == NULL);

  int first = 5;
  llPushFront(&list, &first);
  assert(list.head->data == &first);
  assert(list.head->next->data == &middle);

  int last = 42;
  llPush(&list, &last);
  assert(list.head->next->next->data == &last);
  assert(llContains(&list, &last));

  assert(llPopLast(&list) == &last);
  assert(llPop(&list) == &first);

  assert(llContains(&list, &last) == false);
  assert(list.head->data == &middle);
  assert(list.head->next == NULL);

  freeList(&list);
}
