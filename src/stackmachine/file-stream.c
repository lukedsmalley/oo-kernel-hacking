#ifndef __FILE_STREAM__
#define __FILE_STREAM__

#include <stdio.h>

#include "types.c"

Boolean readFileStreamByte(ByteStream *stream) {
  stream->hasValue = fread(&stream->value, 1, 1, stream->handle) > 0;
  return stream->hasValue;
}

ByteStream openFileStream(Char *filename) {
  return (ByteStream) {
    .handle = fopen(filename, "rb"),
    .hasValue = false,
    .value = 0,
    .reader = readFileStreamByte
  };
}

Void closeFileStream(ByteStream stream) {
  fclose(stream.handle);
}

#endif
