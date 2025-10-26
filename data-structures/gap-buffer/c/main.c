#include <assert.h>
#include <stdio.h>

#include "../../../lib/munit/munit.h"
#include "gap_buffer.h"

const char a = 'a';
const char b = 'b';
const char c = 'c';
const char d = 'd';
const char x = 'x';

typedef struct {
  size_t idx;
  char value;
} ExpectedValue;

typedef struct {
  size_t count;
  size_t gapStart;
  size_t gapEnd;
  size_t capacity;
  const ExpectedValue *values;
} ExpectedBufferState;

static void assertBuffer(GapBuffer *buffer, ExpectedBufferState state) {
  munit_assert_size(buffer->count, ==, state.count);
  munit_assert_size(buffer->gapStart, ==, state.gapStart);
  munit_assert_size(buffer->gapEnd, ==, state.gapEnd);
  munit_assert_size(buffer->capacity, ==, state.capacity);

  if (state.values == NULL) {
    munit_assert_null(buffer->values);
  } else {
    for (size_t i = 0; i < buffer->count; ++i) {
      munit_assert_char(*(char*)buffer->values[state.values[i].idx], ==, state.values[i].value);
    }
  }
}

static void *emptyBuffer(const MunitParameter params[], void *userData) {
  GapBuffer *buffer = munit_calloc(1, sizeof(GapBuffer));
  initGapBuffer(buffer);

  assertBuffer(buffer, (ExpectedBufferState) {0, 0, 0, 0, NULL});

  return buffer;
}

static void *bufferWithGap(const MunitParameter params[], void *userData) {
  GapBuffer *buffer = calloc(1, sizeof(GapBuffer));
  initGapBuffer(buffer);

  gapBufferInsert(buffer, &a, 0);
  gapBufferInsert(buffer, &b, 1);
  gapBufferInsert(buffer, &c, 2);
  gapBufferInsert(buffer, &d, 3);
  gapBufferInsert(buffer, &x, 2);

  assertBuffer(buffer, (ExpectedBufferState) {
    5, 3, 5, 8,
    (ExpectedValue[]) {
      { 0, a }, { 1, b }, { 2, x }, { 6, c }, { 7, d },
    },
  });

  return buffer;
}

static void afterTest(void *fixture) {
  freeGapBuffer(fixture);
  free(fixture);
}

static MunitResult testInsertEmptyBuffer(const MunitParameter params[], void *fixture) {
  GapBuffer *buffer = (GapBuffer*)fixture;

  gapBufferInsert(buffer, &a, 0);
  gapBufferInsert(buffer, &b, 1);

  assertBuffer(buffer, (ExpectedBufferState) {
    2, 2, 7, 8,
    (ExpectedValue[]) { { 0, a }, { 1, b } },
  });

  return MUNIT_OK;
}

static MunitResult testInsert(const MunitParameter params[], void *fixture) {
  GapBuffer *buffer = (GapBuffer*)fixture;

  char y = 'y';
  size_t position;
  ExpectedBufferState state;
  const char* positionStr = munit_parameters_get(params, "position");

  switch (positionStr[0]) {
    case 'f': // first
      position = 0;
      state = (ExpectedBufferState) {
        6, 1, 2, 8,
        (ExpectedValue[]) {
          { 0, y }, { 3, a }, { 4, b }, { 5, x }, { 6, c }, { 7, d },
        },
      };
      break;
    case 'b': // before_gap
      position = 1;
      state = (ExpectedBufferState) {
        6, 2, 3, 8,
        (ExpectedValue[]) {
          { 0, a }, { 1, y }, { 4, b }, { 5, x }, { 6, c }, { 7, d },
        },
      };
      break;
    case 'g': // gap_start
      position = 3;
      state = (ExpectedBufferState) {
        6, 4, 5, 8,
        (ExpectedValue[]) {
          { 0, a }, { 1, b }, { 2, x }, { 3, y }, { 6, c }, { 7, d },
        },
      };
      break;
    case 'a': // after_gap
      position = 4;
      state = (ExpectedBufferState) {
        6, 5, 6, 8,
        (ExpectedValue[]) {
          { 0, a }, { 1, b }, { 2, x }, { 3, c }, { 4, y }, { 7, d },
        },
      };
      break;
    case 'l': // last
      position = 5;
      state = (ExpectedBufferState) {
        6, 6, 7, 8,
        (ExpectedValue[]) {
          { 0, a }, { 1, b }, { 2, x }, { 3, c }, { 4, d }, { 5, y },
        },
      };
      break;
    default:
      munit_errorf("unexpected test case: %s\n", positionStr);
      return MUNIT_ERROR;
  }

  gapBufferInsert(buffer, &y, position);

  assertBuffer(buffer, state);

  return MUNIT_OK;
}

static MunitResult testRemove(const MunitParameter params[], void *fixture) {
  GapBuffer *buffer = (GapBuffer*)fixture;

  size_t position;
  ExpectedBufferState state;
  const char* positionStr = munit_parameters_get(params, "position");

  switch (positionStr[0]) {
    case 'f': // first
      position = 0;
      state = (ExpectedBufferState) {
        4, 0, 3, 8,
        (ExpectedValue[]) { { 4, b }, { 5, x }, { 6, c }, { 7, d } },
      };
      break;
    case 'b': // before_gap
      position = 1;
      state = (ExpectedBufferState) {
        4, 1, 4, 8,
        (ExpectedValue[]) { { 0, a }, { 5, x }, { 6, c }, { 7, d } },
      };
      break;
    case 'g': // gap_start (the char directly before gap starts)
      position = 2;
      state = (ExpectedBufferState) {
        4, 2, 5, 8,
        (ExpectedValue[]) { { 0, a }, { 1, b }, { 6, c }, { 7, d } },
      };
      break;
    case 'a': // after_gap
      position = 3;
      state = (ExpectedBufferState) {
        4, 3, 6, 8,
        (ExpectedValue[]) { { 0, a }, { 1, b }, { 2, x }, { 7, d } },
      };
      break;
    case 'l': // last
      position = 4;
      state = (ExpectedBufferState) {
        4, 4, 7, 8,
        (ExpectedValue[]) { { 0, a }, { 1, b }, { 2, x }, { 3, c } },
      };
      break;
    default:
      munit_errorf("unexpected test case: %s\n", positionStr);
      return MUNIT_ERROR;
  }

  gapBufferRemove(buffer, position);

  assertBuffer(buffer, state);

  return MUNIT_OK;
}

static MunitResult testRetrieve(const MunitParameter params[], void *fixture) {
  GapBuffer *buffer = (GapBuffer*)fixture;
  const char expectedValue[] = { a, b, x, c, d };

  munit_assert_size(buffer->count, ==, 5);
  for (size_t i = 0; i < buffer->count; ++i) {
    munit_assert_char(*(char*)gapBufferRetrieve(buffer, i), ==, expectedValue[i]);
  }

  return MUNIT_OK;
}

static char* positionParams[] = {
  (char*) "first",
  (char*) "before_gap",
  (char*) "gap_start",
  (char*) "after_gap",
  (char*) "last",
  NULL,
};

int main(int argc, char * const* argv) {
  static const MunitTest tests[] = {
    {
      "/insert_empty_buffer",
      testInsertEmptyBuffer,
      emptyBuffer,
      afterTest,
      MUNIT_TEST_OPTION_NONE,
      NULL,
    },
    {
      "/insert",
      testInsert,
      bufferWithGap,
      afterTest,
      MUNIT_TEST_OPTION_NONE,
      (static MunitParameterEnum[]) {
        { (char*) "position", positionParams },
        { NULL, NULL },
      },
    },
    {
      "/remove",
      testRemove,
      bufferWithGap,
      afterTest,
      MUNIT_TEST_OPTION_NONE,
      (static MunitParameterEnum[]) {
        { (char*) "position", positionParams },
        { NULL, NULL },
      },
    },
    {
      "/retrieve",
      testRetrieve,
      bufferWithGap,
      afterTest,
      MUNIT_TEST_OPTION_NONE,
      NULL,
    },
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
  };
  static const MunitSuite suite = {
    "/gap-buffer",
    tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE,
  };

  return munit_suite_main(&suite, NULL, argc, argv);
}
