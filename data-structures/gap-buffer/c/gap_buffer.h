#ifndef fg_gap_buffer_h
#define fg_gap_buffer_h

typedef void* GapBufferValue;

typedef struct {
  size_t capacity;
  size_t count;
  size_t gapStart;
  size_t gapEnd;
  GapBufferValue *values;
} GapBuffer;

void initGapBuffer(GapBuffer *buffer);
void freeGapBuffer(GapBuffer *buffer);
void gapBufferInsert(GapBuffer *buffer, GapBufferValue data, size_t position);
void gapBufferRemove(GapBuffer *buffer, size_t position);
GapBufferValue gapBufferRetrieve(GapBuffer *buffer, size_t position);

#endif
