#include <assert.h>
#include <memory.h>
#include <stddef.h>
#include <stdio.h>

#include "../../common/c/memory.h"
#include "gap_buffer.h"

const size_t insertOffset = 0;
const size_t removeOffset = 1;

void initGapBuffer(GapBuffer *buffer) {
  buffer->capacity = 0;
  buffer->count = 0;
  buffer->gapStart = 0;
  buffer->gapEnd = 0;
  buffer->values = NULL;
}

void freeGapBuffer(GapBuffer *buffer) {
  free(buffer->values);
  initGapBuffer(buffer);
}

void moveGap(GapBuffer *buffer, size_t position, size_t targetOffset) {
  size_t target = buffer->gapStart - targetOffset;
  if (position != target) {
    if (position < target) {
      size_t moveSize = target - position;
      buffer->gapEnd -= moveSize;
      memcpy(buffer->values + buffer->gapEnd + 1, buffer->values + position + targetOffset, moveSize*sizeof(GapBufferValue));
    } else {
      size_t moveSize = position - target;
      memcpy(buffer->values + buffer->gapStart, buffer->values + buffer->gapEnd + 1, moveSize*sizeof(GapBufferValue));
      buffer->gapEnd += moveSize;
    }
  }
}

void gapBufferInsert(GapBuffer *buffer, GapBufferValue value, size_t position) {
  assert(position <= buffer->count);

  if (buffer->count >= buffer->capacity) {
    size_t oldCapacity = buffer->capacity;
    buffer->capacity = GROW_CAPACITY(buffer->capacity);
    buffer->values = GROW_ARRAY(GapBufferValue, buffer->values, buffer->capacity);
    buffer->gapEnd = buffer->capacity - 1;
  }

  moveGap(buffer, position, insertOffset);

  buffer->values[position] = value;
  buffer->count++;
  buffer->gapStart = position + 1;
}

void gapBufferRemove(GapBuffer *buffer, size_t position) {
  assert(position < buffer->count);
  moveGap(buffer, position, removeOffset);
  buffer->count--;
  buffer->gapStart = position;
}

GapBufferValue gapBufferRetrieve(GapBuffer *buffer, size_t position) {
  assert(position < buffer->count);

  if (position >= buffer->gapStart) {
    position = position + buffer->gapEnd - buffer->gapStart + 1;
  }

  return buffer->values[position];
}

