#ifndef __DEFAULT_HEAP__
#define __DEFAULT_HEAP__

#include "types.c"
#include "heap.c"

#define DEFAULT_HEAP_SIZE 4096

byte defaultHeapBuffer[DEFAULT_HEAP_SIZE];
Heap defaultHeap;

void createDefaultHeap() {
  defaultHeap = createHeap(defaultHeapBuffer, DEFAULT_HEAP_SIZE);
}

void *allocFromDefaultHeap(ulong size) {
  return allocFromHeap(&defaultHeap, size);
}

void deallocFromDefaultHeap(void *allocation) {
  deallocFromHeap(&defaultHeap, allocation);
}

void *reallocFromDefaultHeap(void *allocation, ulong size) {
  return reallocFromHeap(&defaultHeap, allocation, size);
}

#endif
