#ifndef fg_linked_list_h
#define fg_linked_list_h

#include <stdbool.h>

typedef void* ListValue;

// the Linux impl of generic list uses some arcane pointer magic to get the embedding struct
// so that you can embedd the Node into your Data struct instead of the data in your Node
// see: https://github.com/torvalds/linux/blob/45db3ab70092637967967bfd8e6144017638563c/include/linux/container_of.h#L10
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
