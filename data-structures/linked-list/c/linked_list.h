#ifndef fg_linked_list_h
#define fg_linked_list_h

#include <stdbool.h>

typedef void* ListValue;

typedef struct LinkedListNode {
  struct LinkedListNode *next;
  ListValue data;
} LinkedListNode;

typedef struct {
  LinkedListNode *head;
} LinkedList;

void freeList(LinkedList *list);
void llPushFront(LinkedList *list, ListValue data);
void llPush(LinkedList *list, ListValue data);
ListValue llPop(LinkedList *list);
ListValue llPopLast(LinkedList *list);
bool llContains(LinkedList *list, ListValue data);

#endif
