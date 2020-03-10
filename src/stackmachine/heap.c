#ifndef __HEAP__
#define __HEAP__

#include "types.c"

#define HEAP_SIZE 128

typedef struct {
  ulong clientCount;
  void *value;
} Reference;

Reference referenceTable[HEAP_SIZE];
ulong nextFreeReferenceIndex = 0;
ulong nextReferenceID = 0;
ulong referenceCount = 0;

ulong createObject() {
  Reference *reference = &referenceTable[referenceCount++];
  reference->id = nextReferenceID++;
  reference->clientCount = 0;
  reference->value = reference->id * 4;
  return reference->id;
}

void *getPointerFromReference(ulong id) {
  ulong start = 0;
  ulong end = referenceCount;
  while (start <= end) {
    ulong i = start + ((end - start) / 2);
    if (referenceTable[i].id == id) {
      return referenceTable[i].value;
    } else if (referenceTable[i].id < id) {
      start = i + 1;
    } else {
      end = i - 1;
    }
  }
  return NULL;
}

#endif
