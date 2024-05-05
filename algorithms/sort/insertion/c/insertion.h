#ifndef fg_insertion_sort_h
#define fg_insertion_sort_h

#include "../../../../data-structures/linked-list/c/linked_list.h"

typedef bool (*ComparisonFn)(LinkedListNode *a, LinkedListNode *b);

void insertionSort(LinkedList *list, ComparisonFn comp);

#endif
