#ifndef __STREAM__
#define __STREAM__

#include "types.c"
#include "list.c"

typedef struct {
  boolean done;
  ulong value;
} StreamItem;

typedef struct {
  boolean done;
  List values;
} StreamItemList;

typedef struct {
  byte itemSize;
  void *handle;
  StreamItem (*next)(void*);
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
    printf("%d\n", item.value);
    value += item.value << (i * stream.itemSize);
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

StreamItemList readStreamItems(Stream stream, ulong count) {
  List values = { NULL, 0, stream.itemSize };
  for (ulong i = 0; i < count; i++) {
    StreamItem item = stream.next(stream.handle);
    if (item.done) {
      return (StreamItemList) {
        .done = true,
        .values = { NULL, 0, stream.itemSize }
      };
    }
    printf("%d\n", item.value);
    addToList(&values, &item.value, stream.itemSize);
  }
  return (StreamItemList) {
    .done = false,
    .values = values
  };
}

#endif
