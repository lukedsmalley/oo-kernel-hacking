#ifndef __ALLOCATION__
#define __ALLOCATION__

#include "types.c"

#define endof(var) \
  ((void*)&var + sizeof(var))

#define NULL_REF -1

typedef struct {
  ulong used;
  void *value;
} Reference;

typedef struct {
  void *start;
  void *free;
  void *end;
  Reference *refTable;
  Reference *nextFreeRef;
  Reference *refTableEnd;
} Heap;

typedef struct {
  ulong size;
  Reference *reference;
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

ulong allocFromHeap(Heap *heap, ulong size) {
  if (size == 0 ||
      heap->free + size + sizeof(AllocHeader) > heap->end ||
      heap->nextFreeRef->used) {
    return -1;
  }

  Reference *ref = heap->nextFreeRef;
  ++ref->used;
  ref->value = heap->free;

  do {
    ++heap->nextFreeRef;
    if (heap->nextFreeRef == heap->refTableEnd) {
      heap->nextFreeRef = heap->refTable;
    }
    if (!heap->nextFreeRef->used) {
      break;
    }
  } while (heap->nextFreeRef != ref);

  AllocHeader header = { size, ref };
  pushMemory(&heap->free, &header, endof(header));
  pushZeroBlock(&heap->free, size);

  return (ref - heap->refTable) / sizeof(Reference);
}

void deallocFromHeap(Heap *heap, ulong ref) {
  if (ref == -1) {
    return;
  }
  void *start = allocation - sizeof(AllocHeader);
  ulong size = ((AllocHeader*)start)->size;
  pushMemory(&start, allocation + size, buffer->free);
  buffer->free -= size + sizeof(AllocHeader);
}

/*void *reallocFromBuffer(AllocBuffer *buffer, const void *allocation, AllocHeaderUpdate headerUpdate) {
  AllocHeader header = { 0, 0 };
  if (allocation != NULL) {
    header = *(AllocHeader*)(allocation - sizeof(AllocHeader));
  }
  void *reallocation = allocFromBuffer(buffer, (AllocHeader) {
    .size = headerUpdate.size == NULL ? header.size : *headerUpdate.size,
    .type = headerUpdate.type == NULL ? header.type : *headerUpdate.type,
  });
  if (allocation == NULL) {
    return reallocation;
  }
  ulong minSize = header.size;
  if (headerUpdate.size != NULL && *headerUpdate.size < minSize) {
    minSize = *headerUpdate.size;
  }
  moveMemory(reallocation, allocation, allocation + minSize);
  deallocFromBuffer(buffer, allocation);
  return reallocation;
}*/

#endif
