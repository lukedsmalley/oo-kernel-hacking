#ifndef __FILE_STREAM__
#define __FILE_STREAM__

#include <stdio.h>

#include "types.c"
#include "stream.c"

boolean readItemFromFile(ulong *value, FILE *handle) {
  byte readValue;
  ulong count = fread(&readValue, 1, 1, handle);
  *value = readValue;
  return count >= 0;
}

Stream openFileStream(const char *filename) {
  return (Stream) {
    .valueSize = sizeof(byte),
    .handle = fopen(filename, "rb"),
    .next = readItemFromFile
  };
}

void closeFileStream(Stream stream) {
  fclose(stream.handle);
}

#endif
