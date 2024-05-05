#ifndef fg_a_star_h
#define fg_a_star_h

#include <stdint.h>

#include "../../../data-structures/linked-list/c/linked_list.h"

typedef struct PathNode {
  uint32_t id;
  uint32_t x;
  uint32_t y;
  float f;
  float g;
  struct PathNode *pred;
  LinkedList neighbors;
} PathNode;

void aStar();

#endif
