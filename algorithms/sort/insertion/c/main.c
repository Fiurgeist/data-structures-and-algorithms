#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "insertion.h"

static bool smallest(LinkedListNode *a, LinkedListNode *b) {
  return *(int*)a->data < *(int*)b->data;
}

int main(int argc, const char *argv[]) {
  LinkedList list = {.head = NULL};

  int first = 5;
  int middle = 23;
  int last = 42;
  llPushFront(&list, &first);
  llPushFront(&list, &middle);
  llPushFront(&list, &last);

  printf("before\n");
  for (LinkedListNode *node = list.head; node != NULL; node = node->next) {
    printf("%d\n", *(int*)node->data);
  }

  insertionSort(&list, smallest);
  printf("after\n");
  for (LinkedListNode *node = list.head; node != NULL; node = node->next) {
    printf("%d\n", *(int*)node->data);
  }

  insertionSort(&list, smallest);
  printf("after (already sorted)\n");
  for (LinkedListNode *node = list.head; node != NULL; node = node->next) {
    printf("%d\n", *(int*)node->data);
  }
}
