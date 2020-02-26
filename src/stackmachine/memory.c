#include "types.c"

#define NULL 0
#define MEMORY_SIZE 4096

byte memory[MEMORY_SIZE];
ulong freeMemoryOffset = 0;

void initializeMemory() {
  for (ulong i = 0 ; i < MEMORY_SIZE; i++) {
    memory[i] = 0;
  }
}

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
  if (freeMemoryOffset + size + 8 > MEMORY_SIZE) {
    return NULL;
  }
  writeLongUnchecked(memory, freeMemoryOffset, size);
  freeMemoryOffset += size + 8;
  return &memory[freeMemoryOffset - size];
}

void free(void *allocation) {
  ulong offset = (byte*)allocation - memory;
  ulong end = offset + readLongUnchecked(memory, offset - 8);
  while (offset < end) {
    memory[offset++] = memory[end];
    memory[end++] = 0;
  }
}
