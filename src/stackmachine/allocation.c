#ifndef __ALLOCATION__
#define __ALLOCATION__

#include "types.c"

typedef struct {
  byte *data;
  ulong size;
  ulong usedSize;
} AllocationBuffer;

typedef struct {
  ulong size;
  ulong type;
} AllocationHeader;

typedef struct {
  ulong *size;
  ulong *type;
} AllocationHeaderUpdate;

void writeLongUnchecked(byte *buffer, ulong offset, long value) {
  for (byte i = 8; i--;) {
    buffer[offset + i] = value >> (i * 8);
  }
}

long readLongUnchecked(const byte *buffer, ulong offset) {
  return (long)buffer[offset] + ((long)buffer[++offset] << 8) +
    ((long)buffer[++offset] << 16) + ((long)buffer[++offset] << 24) +
    ((long)buffer[++offset] << 32) + ((long)buffer[++offset] << 40) +
    ((long)buffer[++offset] << 48) + ((long)buffer[++offset] << 56);
}

byte *copy(byte *to, const byte *from, ulong size) {
  for (ulong i = 0; i < size; i++) {
    to[i] = from[i];
  }
  return to;
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

void *reallocateFromBuffer(AllocationBuffer *buffer, const void *allocation, AllocationHeaderUpdate headerUpdate) {
  AllocationHeader oldHeader = getAllocationHeader(buffer, allocation);
  byte *reallocation = allocateFromBuffer(buffer, (AllocationHeader) {
    .size = headerUpdate.size == NULL ? oldHeader.size : *headerUpdate.size,
    .type = headerUpdate.type == NULL ? oldHeader.type : *headerUpdate.type,
  });
  if (allocation == NULL) {
    return reallocation;
  }
  ulong minSize = oldHeader.size;
  if (headerUpdate.size != NULL && *headerUpdate.size < minSize) {
    minSize = *headerUpdate.size;
  }
  for (ulong i = 0; i < minSize; i++) {
    reallocation[i] = ((byte*)allocation)[i];
  }
  deallocateFromBuffer(buffer, allocation);
  return reallocation;
}

#endif
