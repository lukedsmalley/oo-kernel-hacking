#ifndef __HEAP__
#define __HEAP__

#include "types.c"

typedef struct {
  Any *start;
  Any *free;
  Any *end;
} Heap;

typedef struct {
  Any *end;
  ULong used;
} AllocHeader;

Void moveMemory(Any *dest, Any *from, Any *to) {
  while (from < to) {
    *((Byte*)dest++) = *((Byte*)from++);
  }
}

Heap createHeap(Any *buffer, ULong size) {
  AllocHeader *header = buffer;
  header->end = buffer + size;
  header->used = 0;
  return (Heap) { buffer, buffer, header->end };
}

Any *allocFromHeap(Heap *heap, ULong size) {
  if (size == 0 || heap->free + size + sizeof(AllocHeader) > heap->end) {
    return NULL;
  }

  Any *allocation = heap->start;
  do {
    AllocHeader *header = allocation;
    ULong allocationSize = header->end - allocation;

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

Void deallocFromHeap(Heap *heap, Any *allocation) {
  if (allocation == NULL) {
    return;
  }

  AllocHeader *header = allocation - sizeof(AllocHeader);

  if (header->used) {
    --header->used;

    //Combine consecutive free cells
    if (!header->used) {
      Any *end = header->end;
      while (!((AllocHeader*)end)->used) {
        end = ((AllocHeader*)end)->end;
      }
      header->end = end;
    }
  }
}

Void shareAlloc(Any *allocation) {
  if (allocation == NULL) {
    return;
  }  
  ++((AllocHeader*)(allocation - sizeof(AllocHeader)))->used;
}

Any *reallocFromHeap(Heap *heap, Any *allocation, ULong size) {
  Any *reallocation = allocFromHeap(heap, size);

  if (reallocation == NULL) {
    return NULL;
  }

  if (allocation != NULL) {
    AllocHeader *header = allocation - sizeof(AllocHeader);
    ULong allocationSize = header->end - allocation + sizeof(AllocHeader);
    moveMemory(reallocation, allocation, allocation + (size < allocationSize ? size : allocationSize));
  }

  return reallocation;
}

#endif
