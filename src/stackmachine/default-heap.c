#ifndef __DEFAULT_HEAP__
#define __DEFAULT_HEAP__

#include "heap.c"

#define DEFAULT_HEAP_SIZE 4096

Byte defaultHeapBuffer[DEFAULT_HEAP_SIZE];
Heap defaultHeap;

void createDefaultHeap() {
  defaultHeap = createHeap(defaultHeapBuffer, DEFAULT_HEAP_SIZE);
}

void *allocFromDefaultHeap(ULong size) {
  return allocFromHeap(&defaultHeap, size);
}

void deallocFromDefaultHeap(void *allocation) {
  deallocFromHeap(&defaultHeap, allocation);
}

void *reallocFromDefaultHeap(void *allocation, ULong size) {
  return reallocFromHeap(&defaultHeap, allocation, size);
}

#endif
