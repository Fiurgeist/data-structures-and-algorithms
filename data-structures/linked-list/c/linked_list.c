#include <stddef.h>

#include "../../common/c/memory.h"
#include "linked_list.h"

static LinkedListNode* newNode(ListValue data, LinkedListNode *next) {
  LinkedListNode *node = ALLOCATE(LinkedListNode, 1);
  node->data = data;
  node->next = next;

  return node;
}

void freeList(LinkedList *list) {
  LinkedListNode *entry = list->head;
  while (entry != NULL) {
    LinkedListNode *next = entry->next;
    free(entry);
    entry = next;
  }
  list->head = NULL;
}

void llPushFront(LinkedList *list, ListValue data) {
  LinkedListNode *node = newNode(data, list->head);
  list->head = node;
}

void llPush(LinkedList *list, ListValue data) {
  LinkedListNode *node = newNode(data, NULL);

  if (list->head == NULL) {
    list->head = node;
  } else {
    LinkedListNode *last = list->head;
    while (last->next != NULL) {
      last = last->next;
    }
    last->next = node;
  }
}

ListValue llPop(LinkedList *list) {
  if (list->head == NULL) {
    return NULL;
  }

  LinkedListNode *first = list->head;
  list->head = first->next;

  ListValue data = first->data;
  free(first);
  return data;
}

ListValue llPopLast(LinkedList *list) {
  if (list->head == NULL) {
    return NULL;
  } else {
    LinkedListNode *prev = list->head;
    LinkedListNode *last = list->head;
    while (last->next != NULL) {
      prev = last;
      last = last->next;
    }

    prev->next = NULL;

    ListValue data = last->data;
    free(last);
    return data;
  }
}

bool llContains(LinkedList *list, ListValue data) {
  if (list->head == NULL) {
    return false;
  } else {
    LinkedListNode *cur = list->head;
    while (cur != NULL) {
      if (cur->data == data) {
        return true;
      }

      cur = cur->next;
    }

    return false;
  }
}
