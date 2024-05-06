#ifdef _WINDOWS
#include <windows.h>
#else
#define _POSIX_C_SOURCE 200112L // for nanosleep from time.h
#define Sleep(x) \
  ({ \
    struct timespec ts = {.tv_sec = 0, .tv_nsec = (x)*1000000}; \
    nanosleep(&ts, NULL); \
  })
#endif

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "../../../data-structures/hash-set/c/set.h"
#include "../../sort/insertion/c/insertion.h"
#include "a_star.h"

#define CLOCKS_PER_MILLI (CLOCKS_PER_SEC / 1000)

#define PRINT_EXPLORATION
#define DELAY 3

const char START = 'S';
const char GOAL = 'G';
const char WALL = '#';

const char *field[] = {
  "..............#..............................................................",
  "..........#...#..............................................................",
  "..........#...#............##################################################",
  "......S...#...#............#.................................................",
  "...########...#...#........#.................................................",
  "..........#...#...#........#.................................................",
  "#####.....#.......#........#.................................................",
  "..........#.......#..#.....#...................#.............................",
  "...####...#.......#..#.....#...................#.............................",
  "..........#.......#..#.....#...................#.............................",
  "....#..####..........#.....#.....######........#........#....................",
  "....#.....#..........#................#........#...#....#....................",
  "....#.....#..........#................#........#...#....#....................",
  "....#................#...########.....#........#...#....#............######..",
  "....#######..........#................#........#...#....#............#....#..",
  "....#................#................#........#...#....#............#....#..",
  "....#.................................#............#....#............#....#..",
  "....#.................................#............#....#............#.G..#..",
  "....#.................................#............#....#............#....#..",
  ".........##############################............#.........................",
};

static double millis() {
  return (double)clock() / CLOCKS_PER_MILLI;
}

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

static float heuristic(PathNode *node, PathNode *goal) {
  // manhatten distance
  // return abs(node->x - goal->x) + abs(node->y - goal->y);

  int dx = abs(node->x - goal->x);
  int dy = abs(node->y - goal->y);
  // octile distance:
  // cost * (dx + dy) + (cost_diag - 2 * cost) * min(dx, dy)
  return (dx + dy) - 0.41 * (dx < dy ? dx : dy);
}

static void maybeAddNeighbor(LinkedList *neighbors, PathNode *neighbor) {
  if (field[neighbor->y][neighbor->x] != WALL) {
    llPushFront(neighbors, neighbor);
  }
}

void aStar() {
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

  printf("\033[2J");
  printf("\033[%d;%dH", 1, 1);
  for(int i = 0; i < height; ++i) {
    printf("%s\n", field[i]);
  }
  printf("\033[%d;%dH", height + 1, 1);
  printf("S: start; G: goal; ");
#ifdef PRINT_EXPLORATION
  printf("\033[48;2;123;123;123m \033[0m: fully explored nodes; ");
  printf("\033[48;2;150;150;150m \033[0m: partially explored nodes; ");
#endif
  printf("\033[48;2;65;152;10m \033[0m: shortest path found\n");

  int counter = 0;
  double timeStart = millis();

  LinkedList openList = {.head = NULL};
  IntSet closedSet = {.entries = NULL, .capacity = 0, .count = 0};
  llPushFront(&openList, start);

  while (openList.head != NULL) {
    PathNode *current = llPop(&openList);
    if (current->id == goal->id) {
      break;
    }

    for (LinkedListNode *neighbor = current->neighbors.head; neighbor != NULL; neighbor = neighbor->next) {
#ifdef DELAY
      Sleep(DELAY);
#endif
      counter++;
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
#ifdef PRINT_EXPLORATION
        printf("\033[%d;%dH", node->y + 1, node->x + 1);
        printf("\033[48;2;150;150;150m%c\033[0m", field[node->y][node->x]);
        fflush(stdout);
#endif
      }
    }

    insertionSort(&openList, smallestF);
    setAdd(&closedSet, &current->id);
#ifdef PRINT_EXPLORATION
    printf("\033[%d;%dH", current->y + 1, current->x + 1);
    printf("\033[48;2;123;123;123m%c\033[0m", field[current->y][current->x]);
    fflush(stdout);
#endif
  }

  double timeEnd = millis();

  freeList(&openList);
  freeSet(&closedSet);

  PathNode *path = goal;
  while (path != NULL) {
    printf("\033[%d;%dH", path->y + 1, path->x + 1);
    printf("\033[48;2;65;152;10m%c\033[0m", field[path->y][path->x]);
    path = path->pred;
  }
  printf("\033[%d;%dH", height + 2, 1);

#if !defined PRINT_EXPLORATION && !defined DELAY
  printf("path finding took: %.3fms\n", timeEnd - timeStart);
#endif
}
