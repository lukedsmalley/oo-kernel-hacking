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

Heap createHeap(byte *buffer, ulong size) {
  AllocHeader *header = buffer;
  header->end = buffer + size;
  header->used = 0;
  return (Heap) { buffer, buffer, header->end };
}

void *allocFromHeap(Heap *heap, ulong size) {
  if (size == 0 || heap->free + size + sizeof(AllocHeader) > heap->end) {
    return NULL;
  }

  void *allocation = heap->start;
  do {
    AllocHeader *header = allocation;
    ulong allocationSize = header->end - allocation;

    if (!header->used) {
      //Combine consecutive free cells
      while (header->end < heap->end) {
        if (((AllocHeader*)header->end)->used) {
          break;
        }
        header->end = ((AllocHeader*)header->end)->end;
        allocationSize = header->end - allocation;
      }

      //Allocate from sufficiently-large free cells
      if (allocationSize >= size + sizeof(AllocHeader)) {
        ++header->used;
        header->end = allocation + allocationSize;

        //Create trailing cells after sufficiently-large free cells
        if (allocationSize > size + (2 * sizeof(AllocHeader))) {
          header->end = allocation + size + sizeof(AllocHeader);
          ((AllocHeader*)header->end)->used = 0;
          ((AllocHeader*)header->end)->end = allocation + allocationSize;
        }

        return allocation + sizeof(AllocHeader);
      }
    }

    allocation += allocationSize;
  } while (allocation < heap->end);

  return NULL;
}

void deallocFromHeap(Heap *heap, void *allocation) {
  if (allocation == NULL) {
    return;
  }

  AllocHeader *header = allocation - sizeof(AllocHeader);

  if (header->used) {
    --header->used;

    //Combine consecutive free cells
    if (!header->used) {
      void *end = header->end;
      while (!((AllocHeader*)end)->used) {
        end = ((AllocHeader*)end)->end;
      }
      header->end = end;
    }
  }
}

void shareAlloc(void *allocation) {
  if (allocation == NULL) {
    return;
  }  
  ++((AllocHeader*)(allocation - sizeof(AllocHeader)))->used;
}

void *reallocFromHeap(Heap *heap, void *allocation, ulong size) {
  void *reallocation = allocFromHeap(heap, size);

  if (reallocation != NULL && allocation != NULL) {
    AllocHeader *header = allocation - sizeof(AllocHeader);
    ulong allocationSize = header->end - allocation + sizeof(AllocHeader);
    void *end = allocation + (size < allocationSize ? size : allocationSize);
    while (allocation < end) {
      *((byte*)reallocation++) = *((byte*)allocation++);
    }
  }

  return reallocation;
}

#endif
