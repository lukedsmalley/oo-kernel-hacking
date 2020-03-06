#ifndef __ALLOCATION__
#define __ALLOCATION__

#include "types.c"

#define endof(var) \
  ((void*)&var + sizeof(var))

typedef struct {
  byte *start;
  byte *free;
  byte *end;
} AllocBuffer;

typedef struct {
  ulong size;
  ulong type;
} AllocHeader;

typedef struct {
  ulong *size;
  ulong *type;
} AllocHeaderUpdate;

void moveMemory(const void *dest, const void *from, const void *to) {
  while (from < to) {
    *((byte*)dest++) = *((byte*)from++);
  }
}

void pushMemory(const void **dest, const void *from, const void *to) {
  while (from < to) {
    *((byte*)*dest++) = *((byte*)from++);
  }
}

void pushZeroBlock(const void **dest, ulong size) {
  void *end = *dest + size;
  while (*dest < end) {
    *((byte*)*dest++) = 0;
  }
}

void *allocFromBuffer(AllocBuffer *buffer, AllocHeader header) {
  if (header.size == 0 ||
      buffer->free + header.size + sizeof(AllocHeader) > buffer->end) {
    return NULL;
  }
  pushMemory(&buffer->free, &header, endof(header));
  pushZeroBlock(&buffer->free, header.size);
  return &buffer->free - header.size;
}

void deallocFromBuffer(AllocBuffer *buffer, const void *allocation) {
  if (allocation == NULL) {
    return;
  }
  void *start = allocation - sizeof(AllocHeader);
  ulong size = ((AllocHeader*)start)->size;
  pushMemory(&start, allocation + size, buffer->free);
  buffer->free -= size + sizeof(AllocHeader);
}

void *reallocFromBuffer(AllocBuffer *buffer, const void *allocation, AllocHeaderUpdate headerUpdate) {
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
}

#endif
