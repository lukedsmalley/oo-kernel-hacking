#include "types.c"
#include "memory.c"

typedef struct {
  void *handle;
  boolean (*hasNext)(void*);
  byte (*next)(void*);
} Stream;

byte streamHasBytes(const Stream *in) {
  return in->hasNext(in->handle);
}

byte readByteFromStream(const Stream *in) {
  return in->next(in->handle);
}

long *readLongFromStream(const Stream *in) {
  long value = 0;
  for (byte i = 0; i < 8; i++) {
    if (!streamHasBytes(in)) {
      return NULL;
    }
    value += readByteFromStream(in) << (i * 8);
  }
  return clone(&value, sizeof(long));
}
