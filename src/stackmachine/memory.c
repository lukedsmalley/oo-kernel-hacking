#include "types.c"

#define NULL 0
#define MEMORY_SIZE 4096

byte memory[MEMORY_SIZE];
ulong freeMemoryOffset = 0;

void writeLongUnchecked(byte *buffer, ulong offset, long value) {
  for (byte i = 8; i--;) {
    buffer[offset + i] = value >> (i * 8);
  }
}

long readLongUnchecked(const byte *buffer, ulong offset) {
  return buffer[offset] + (buffer[++offset] << 8) +
    (buffer[++offset] << 16) + (buffer[++offset] << 24) +
    (buffer[++offset] << 32) + (buffer[++offset] << 40) +
    (buffer[++offset] << 48) + (buffer[++offset] << 56);
}

void *allocate(ulong size) {
  if (size == 0) {
    return NULL;
  }
  ulong allocationOffset = freeMemoryOffset;
  freeMemoryOffset += size + 8;
  if (freeMemoryOffset > MEMORY_SIZE) {
    return NULL;
  }
  writeLongUnchecked(memory, allocationOffset, size);
  allocationOffset += 8;
  for (ulong i = 0; i < size; i++) {
    memory[allocationOffset + i] = 0;
  }
  return &memory[allocationOffset];
}

void deallocate(const void *allocation) {
  if (allocation == NULL) {
    return;
  }
  ulong offset = (byte*)allocation - memory - 8;
  ulong end = offset + 8 + readLongUnchecked(memory, offset);
  while (end < freeMemoryOffset) {
    memory[offset++] = memory[end];
    memory[end++] = 0;
  }
  freeMemoryOffset = end;
}

void *reallocate(const void *allocation, ulong size) {
  byte *reallocation = allocate(size);
  if (allocation == NULL) {
    return reallocation;
  }
  ulong minSize = readLongUnchecked(memory, (byte*)allocation - memory - 8);
  if (size < minSize) {
    minSize = size;
  }
  for (ulong i = 0; i < minSize; i++) {
    reallocation[i] = ((byte*)allocation)[i];
  }
  deallocate(allocation);
  return reallocation;
}
