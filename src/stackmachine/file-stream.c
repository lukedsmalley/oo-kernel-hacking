#ifndef __FILE_STREAM__
#define __FILE_STREAM__

#include <stdio.h>

#include "types.c"
#include "stream.c"

StreamItem readItemFromFile(FILE *handle) {
  byte value = 0;
  ulong count = fread(&value, 1, 1, handle);
  return count < 1 ? (StreamItem) {
    .done = true,
    .value = 0
  } : (StreamItem) {
    .done = false,
    .value = value
  };
}

Stream openFileStream(const char *filename) {
  return (Stream) {
    .itemSize = sizeof(byte),
    .handle = fopen(filename, "rb"),
    .next = readItemFromFile
  };
}

void closeFileStream(Stream stream) {
  fclose(stream.handle);
}

#endif
