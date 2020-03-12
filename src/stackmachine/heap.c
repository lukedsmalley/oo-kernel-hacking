#ifndef __ALLOCATION__
#define __ALLOCATION__

#include "types.c"

#define endof(var) \
  ((void*)&var + sizeof(var))

typedef struct {
  void *start;
  void *free;
  void *end;
} Heap;

typedef struct {
  void *end;
  ulong used;
} AllocHeader;

void moveMemory(const void *dest, const void *from, const void *to) {
  while (from < to) {
    *((byte*)dest++) = *((byte*)from++);
  }
}

void pushMemory(const void **dest, const void *from, const void *to) {
  while (from < to) {
    *((byte*)(*dest)++) = *((byte*)from++);
  }
}

void pushZeroBlock(const void **dest, ulong size) {
  void *end = *dest + size;
  while (*dest < end) {
    *((byte*)(*dest)++) = 0;
  }
}

Heap createHeap(byte *buffer, ulong size) {
  AllocHeader *header = buffer;
  header->end = buffer + size;
  header->used = 0;
  return (Heap) { buffer, buffer, header->end };
}

void *allocFromHeap(Heap *heap, ulong size) {
  if (size == 0 || heap->free + size + sizeof(AllocHeader) >= heap->end) {
    return NULL;
  }

  void *allocation = heap->start;
  do {
    AllocHeader *header = allocation;
    void *end = header->end;
    if (!header->used && end - allocation <= size + (2 * sizeof(AllocHeader))) {
      header->end = allocation + size + sizeof(AllocHeader);
      ++header->used;
      ((AllocHeader*)header->end)->used = 0;
      ((AllocHeader*)header->end)->end = end;
      return allocation + sizeof(AllocHeader);
    }
    allocation = end;
  } while (allocation < heap->end);

  return NULL;
}

void deallocFromHeap(Heap *heap, void *allocation) {
  AllocHeader *header = allocation;
  if (header->used) {
    --header->used;
    if (!header->used) {
      void *end = header->end;
      while (!((AllocHeader*)end)->used) {
        end = ((AllocHeader*)end)->end;
      }
      header->end = end;
    }
  }
}

#endif
