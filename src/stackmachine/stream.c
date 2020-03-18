#ifndef __STREAM__
#define __STREAM__

#include "types.c"
#include "list.c"

typedef struct {
  boolean done;
  ulong value;
} StreamItem;

typedef struct {
  byte itemSize;
  void *handle;
  StreamItem (*next)(void *handle);
} Stream;

StreamItem readStreamItem(Stream stream) {
  return stream.next(stream.handle);
}

StreamItem readAndCombineStreamItems(Stream stream, byte count) {
  ulong value = 0;
  for (byte i = 0; i < count; i++) {
    StreamItem item = stream.next(stream.handle);
    if (item.done) {
      return item;
    }
    value += item.value << (i * 8 * stream.itemSize);
  }
  return (StreamItem) {
    .done = false,
    .value = value
  };
}

StreamItem readStreamWord(Stream stream) {
  return readAndCombineStreamItems(stream, 2);
}

StreamItem readStreamDWord(Stream stream) {
  return readAndCombineStreamItems(stream, 4);
}

StreamItem readStreamQWord(Stream stream) {
  return readAndCombineStreamItems(stream, 8);
}

List readStreamItems(Stream stream, ulong count) {
  List values = createListOnDefaultHeap(stream.itemSize);
  for (ulong i = 0; i < count; i++) {
    StreamItem item = stream.next(stream.handle);
    if (item.done) {
      destroyList(values);
      return createListOnDefaultHeap(stream.itemSize);
    }
    addToList(&values, &item.value, stream.itemSize);
  }
  return values;
}

#endif
