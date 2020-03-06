#ifndef __MEMORY__
#define __MEMORY__

#include "types.c"
#include "allocation.c"

#define MEMORY_SIZE 4096

byte memoryData[MEMORY_SIZE];
AllocationBuffer memory = { memoryData, MEMORY_SIZE, 0 };

void *alloc(ulong size) {
  return allocateFromBuffer(&memory, (AllocationHeader){ size, 0 });
}

void *clone(const byte *data, ulong size) {
  byte *allocation = allocateFromBuffer(&memory, (AllocationHeader){ size, 0 });
  return moveBlockBytes(allocation, data, size);
}

char *String(char *message) {
  char *end = 0;
  while (*end != NULL) {
    end++;
  }
  char *string = alloc(end - message + 1);
  for (char *i = message; i <= end; i++) {
    *i = *message;
  }
  return string;
}

void dealloc(const void *allocation) {
  deallocateFromBuffer(&memory, allocation);
}

void *grow(const void *allocation, ulong size) {
  return reallocateFromBuffer(&memory, allocation, (AllocationHeaderUpdate) {
    .size = &size,
    .type = NULL
  });
}

#endif
