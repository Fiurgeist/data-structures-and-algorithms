#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <math.h>

#include "../../../data-structures/hash-set/c/set.h"
#include "../../sort/insertion/c/insertion.h"
#include "a_star.h"

const char START = 'S';
const char GOAL = 'G';
const char WALL = '#';

const char *field[] = {
  "...#G..",
  "...#...",
  ".S.###.",
  ".###.#.",
  "....##.",
  "...#...",
  "...#...",
  ".......",
};

static bool smallestF(LinkedListNode *a, LinkedListNode *b) {
  return ((PathNode*)a->data)->f < ((PathNode*)b->data)->f;
}

static float calcG(PathNode *node, PathNode *prev) {
  float cost = 1;
  if (node->x != prev->x && node->y != prev->y) {
    cost = 1.41;
  }

  return cost + prev->g;
}

static int heuristic(PathNode *node, PathNode *goal) {
  return abs(node->x - goal->x) + abs(node->y - goal->y);
}

void aStar(PathNode *start, PathNode *goal) {
  LinkedList openList = {.head = NULL};
  IntSet closedSet = {.entries = NULL, .capacity = 0, .count = 0};
  llPushFront(&openList, start);

  while (openList.head != NULL) {
    PathNode *current = llPop(&openList);
    if (current->id == goal->id) {
      break;
    }

    for (LinkedListNode *neighbor = current->neighbors.head; neighbor != NULL; neighbor = neighbor->next) {
      PathNode *node = (PathNode*)neighbor->data;
      if (setContains(&closedSet, &node->id)) {
        continue;
      }

      float g = calcG(node, current);
      bool inOpenList = llContains(&openList, node);
      if (inOpenList && g > node->g) {
        continue;
      }

      node->g = g;
      node->f = g + heuristic(node, goal);
      node->pred = current;

      if (!inOpenList) {
        llPushFront(&openList, node);
      }
    }

    insertionSort(&openList, smallestF);
    setAdd(&closedSet, &current->id);
  }

  freeList(&openList);
  freeSet(&closedSet);
}

static void maybeAddNeighbor(LinkedList *neighbors, PathNode *neighbor) {
  if (field[neighbor->y][neighbor->x] != WALL) {
    llPushFront(neighbors, neighbor);
  }
}

void runAStar() {
  int height = sizeof(field) / sizeof(field[0]);
  int width = (int)strlen(field[0]);
  int idSlug = height * width;

  PathNode *start;
  PathNode *goal;

  PathNode nodes[height][width];
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      PathNode p = { .x = x, .y = y, .id = y*idSlug + x, .g = 0, .f = FLT_MAX, .pred = NULL, .neighbors = NULL };
      nodes[y][x] = p;

      if (field[y][x] == START) {
        start = &nodes[y][x];
      } else if (field[y][x] == GOAL) {
        goal = &nodes[y][x];
      }
    }
  }

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      nodes[y][x].neighbors = (LinkedList){.head = NULL};
      if (x > 0) {
        maybeAddNeighbor(&nodes[y][x].neighbors, &nodes[y][x-1]);
        if (y > 0) {
          maybeAddNeighbor(&nodes[y][x].neighbors, &nodes[y-1][x-1]);
        }
        if (y < height - 1) {
          maybeAddNeighbor(&nodes[y][x].neighbors, &nodes[y+1][x-1]);
        }
      }
      if (x < width - 1) {
        maybeAddNeighbor(&nodes[y][x].neighbors, &nodes[y][x+1]);
        if (y > 0) {
          maybeAddNeighbor(&nodes[y][x].neighbors, &nodes[y-1][x+1]);
        }
        if (y < height - 1) {
          maybeAddNeighbor(&nodes[y][x].neighbors, &nodes[y+1][x+1]);
        }
      }
      if (y > 0) {
        maybeAddNeighbor(&nodes[y][x].neighbors, &nodes[y-1][x]);
      }
      if (y < height - 1) {
        maybeAddNeighbor(&nodes[y][x].neighbors, &nodes[y+1][x]);
      }
    }
  }

  aStar(start, goal);

  printf("########\n");
  printf("x - y:\n");
  PathNode *path = goal;
  while (path != NULL) {
    printf("%d - %d\n", path->x, path->y);
    path = path->pred;
  }
  printf("########\n");
}
