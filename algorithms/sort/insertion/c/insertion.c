#include <stddef.h>

#include "insertion.h"

void insertionSort(LinkedList *list, ComparisonFn comp) {
  if (list == NULL || list->head == NULL || list->head->next == NULL) {
    return;
  }

  LinkedListNode *sorted = NULL;
  while (list->head != NULL) {
    LinkedListNode *cur = list->head;
    list->head = list->head->next;

    if (sorted == NULL || comp(cur, sorted)) {
      cur->next = sorted;
      sorted = cur;
    } else {
      LinkedListNode *pos = sorted;
      while (pos != NULL) {
        if (pos->next == NULL || comp(cur, pos->next)) {
          cur->next = pos->next;
          pos->next = cur;

          break;
        }

        pos = pos->next;
      }
    }
  }

  list->head = sorted;
}
