#include "types.c"

#define NULL 0
#define MEMORY_SIZE 4096


typedef struct {
  byte *data;
  ulong size;
  ulong usedSize;
} AllocationBuffer;

typedef struct {
  ulong size;
  byte type;
} AllocationHeader;

typedef struct {
  ulong *size;
  byte *type;
} AllocationHeaderChange;


byte memoryData[MEMORY_SIZE];
AllocationBuffer memory = { memoryData, MEMORY_SIZE, 0 };


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

void *allocateFromBuffer(AllocationBuffer *buffer, AllocationHeader header) {
  if (header.size == 0 || buffer->usedSize + header.size + sizeof(AllocationHeader) > buffer->size) {
    return NULL;
  }
  for (ulong i = 0; i < sizeof(AllocationHeader); i++) {
    buffer->data[buffer->usedSize + i] = ((byte*)&header)[i];
  }
  buffer->usedSize += sizeof(AllocationHeader);
  for (ulong i = 0; i < header.size; i++) {
    buffer->data[buffer->usedSize + i] = 0;
  }
  buffer->usedSize += header.size;
  return &buffer->data[buffer->usedSize - header.size];
}

void deallocateFromBuffer(AllocationBuffer *buffer, const void *allocation) {
  if (allocation == NULL) {
    return;
  }
  ulong offset = (byte*)allocation - buffer->data - sizeof(AllocationHeader);
  ulong end = offset + sizeof(AllocationHeader) + readLongUnchecked(buffer->data, offset);
  while (end < buffer->usedSize) {
    buffer->data[offset++] = buffer->data[end];
  }
  buffer->usedSize = end;
}

AllocationHeader getAllocationHeader(const AllocationBuffer *buffer, const void *allocation) {
  AllocationHeader header = {};
  ulong headerOffset = (byte*)allocation - buffer->data - sizeof(AllocationHeader);
  for (ulong i = 0; i < sizeof(AllocationHeader); i++) {
    ((byte*)&header)[i] = buffer->data[headerOffset + i];
  }
  return header;
}

void *reallocateFromBuffer(const AllocationBuffer *buffer, const void *allocation, AllocationHeaderChange headerChange) {
  AllocationHeader oldHeader = getAllocationHeader(buffer, allocation);
  byte *reallocation = allocateFromBuffer(buffer, (AllocationHeader) {
    .size = headerChange.size == NULL ? oldHeader.size : *headerChange.size,
    .type = headerChange.type == NULL ? oldHeader.type : *headerChange.type,
  });
  if (allocation == NULL) {
    return reallocation;
  }
  ulong minSize = oldHeader.size;
  if (headerChange.size != NULL && *headerChange.size < minSize) {
    minSize = *headerChange.size;
  }
  for (ulong i = 0; i < minSize; i++) {
    reallocation[i] = ((byte*)allocation)[i];
  }
  deallocateFromBuffer(buffer, allocation);
  return reallocation;
}

void *allocate(ulong size) {
  return allocateFromBuffer(&memory, (AllocationHeader){ size, 0 });
}

void deallocate(const void *allocation) {
  deallocateFromBuffer(&memory, allocation);
}

void *reallocate(const void *allocation, ulong size) {
  return reallocateFromBuffer(&memory, allocation, (AllocationHeaderChange) {
    .size = size,
    .type = NULL
  });
}
