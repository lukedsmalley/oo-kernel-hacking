#ifndef __MEMORY__
#define __MEMORY__

#include "types.c"
#include "allocation.c"

#define MEMORY_SIZE 4096

byte memoryData[MEMORY_SIZE];
AllocBuffer memory = { memoryData, memoryData, endof(memoryData) };

void *alloc(ulong size) {
  return allocFromBuffer(&memory, (AllocHeader){ size, 0 });
}

void *clone(const byte *data, ulong size) {
  byte *allocation = allocFromBuffer(&memory, (AllocHeader){ size, 0 });
  moveMemory(allocation, data, data + size);
  return allocation;
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
  deallocFromBuffer(&memory, allocation);
}

void *grow(const void *allocation, ulong size) {
  return reallocFromBuffer(&memory, allocation, (AllocHeaderUpdate) {
    .size = &size,
    .type = NULL
  });
}

#endif
