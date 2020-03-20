#ifndef __STREAM__
#define __STREAM__

#include "types.c"
#include "list.c"
#include "list-macros.c"

typedef struct {
  byte valueSize;
  void *handle;
  boolean (*next)(long *value, void *handle);
} Stream;

typedef boolean (*StreamReader)(ulong *value, Stream stream);
typedef boolean (*ListItemStreamReader)(List(void) *list, ulong index, Stream stream);

boolean readStream(ulong *value, Stream stream) {
  return stream.next(value, stream.handle);
}

boolean readAndCombineStreamItems(ulong *value, Stream stream, byte count) {
  for (byte i = 0; i < count; i++) {
    if (!stream.next(value, stream.handle)) return false;
    if (i < count - 1) *value <<= 8 * stream.valueSize;
  }
}

boolean readStreamWord(ulong *value, Stream stream) {
  return readAndCombineStreamItems(value, stream, 2);
}

boolean readStreamDWord(ulong *value, Stream stream) {
  return readAndCombineStreamItems(value, stream, 4);
}

boolean readStreamQWord(ulong *value, Stream stream) {
  return readAndCombineStreamItems(value, stream, 8);
}

boolean readListFromStream(List(void) *list, ListItemStreamReader itemReader, Stream stream, StreamReader countReader) {
  ulong count;
  if (!countReader(&count, stream)) return false;
  for (ulong i = 0; i < count; i++) {
    if (!itemReader(list, i, stream)) return false;
  }
  return true;
}

#endif
