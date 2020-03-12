#include <stdio.h>

#include "../types.c"
#include "../heap.c"

#define FUN_LONG 0xfedcba9876543210
#define FUN_LONG_BYTES 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe

test int endof_returnsEndPointer() {
  byte value = 0;

  if ((byte*)endof(value) - &value != sizeof(value))
    return !!printf("endof() does not return a pointer to the location after the " \
      "given memory location.\n");

  return 0;
}

test int createHeap_returnsHeap() {
  byte memory[sizeof(AllocHeader)];
  Heap heap = createHeap(memory, sizeof(memory));
  AllocHeader *header = heap.start;

  if (header->end != endof(memory))
    return !!printf("Bad end pointer in placeholder header.\n");

  if (header->used)
    return !!printf("Bad reference count in placeholder header.\n");

  return 0;
}

test int allocFromHeap_doesNothingForZeroSize() {
  return !!printf("Test not yet implemented.\n");
}

test int allocFromHeap_addsToHeapWithExactSpace() {
  byte memory[sizeof(AllocHeader) + sizeof(long)];
  Heap heap = createHeap(memory, sizeof(memory));

  void* allocation = allocFromHeap(&heap, sizeof(long));

  if (allocation == NULL)
    return !!printf("A null reference was returned.\n");

  AllocHeader *header = allocation - sizeof(AllocHeader);

  if (header != memory)
    return !!printf("Heap memory wasn't filled from its left-most point.\n");

  if (header->used != 1)
    return !!printf("Bad reference count in allocation header.\n");

  if (header->end != endof(memory))
    return !!printf("Bad end pointer in allocation header.\n");

  return 0;
}

test int allocFromHeap_addsToHeapConsecutively() {
  byte memory[2 * (sizeof(AllocHeader) + sizeof(long))];
  Heap heap = createHeap(memory, sizeof(memory));

  for (int i = 0; i < 2; i++) {
    void* allocation = allocFromHeap(&heap, sizeof(long));

    if (allocation == NULL)
      return !!printf("A null reference was returned.\n");

    AllocHeader *header = allocation - sizeof(AllocHeader);

    if (header != memory + (i * (sizeof(AllocHeader) + sizeof(long))))
      return !!printf("Heap memory wasn't filled from its left-most point.\n");

    if (header->used != 1)
      return !!printf("Bad reference count in allocation header.\n");

    if (header->end != allocation + sizeof(long))
      return !!printf("Bad end pointer in allocation header.\n");
  }

  return 0;
}

test int allocFromHeap_addsToHeapAfterPopDeallocation() {
  byte memory[2 * (sizeof(AllocHeader) + sizeof(long))];
  Heap heap = createHeap(memory, sizeof(memory));

  void *allocations[2];

  //Allocate items, then deallocate in reverse order
  allocations[0] = allocFromHeap(&heap, sizeof(long));
  allocations[1] = allocFromHeap(&heap, sizeof(long));
  deallocFromHeap(&heap, allocations[1]);
  deallocFromHeap(&heap, allocations[0]);

  allocations[0] = allocFromHeap(&heap, 2 * sizeof(long));

  if (allocations[0] == NULL)
    return !!printf("A null reference was returned.\n");

  AllocHeader *header = allocations[0] - sizeof(AllocHeader);

  if (header != memory)
    return !!printf("Heap memory wasn't filled from its left-most point.\n");

  if (header->used != 1)
    return !!printf("Bad reference count in allocation header.\n");

  if (header->end != memory + sizeof(AllocHeader) + 2 * sizeof(long))
    return !!printf("Bad end pointer in allocation header.\n");

  return 0;
}

test int deallocFromHeap_doesNothingForNullPointer() {
  byte memory[sizeof(AllocHeader) + sizeof(long)];
  Heap heap = createHeap(memory, sizeof(memory));

  //This is a relatively weak test
  deallocFromHeap(&heap, NULL);

  return 0;
}

test int deallocFromHeap_mergesUnusedSpace() {
  byte memory[2 * (sizeof(AllocHeader) + sizeof(long))];
  Heap heap = createHeap(memory, sizeof(memory));

  void *allocations[2];

  for (int i = 0; i < 2; i++) {
    allocations[i] = allocFromHeap(&heap, sizeof(long));
  }

  for (int i = 1; i >= 0; i--) {
    deallocFromHeap(&heap, allocations[i]);

    AllocHeader *header = allocations[i] - sizeof(AllocHeader);

    if (header->used)
      return !!printf("Reference count of allocation %d wasn't changed to reflect dereference.\n", i);

    if (header->end != endof(memory))
      return !!printf("Bad end pointer in allocation %d after deallocation.\n", i);
  }

  return 0;
}

test int deallocFromHeap_doesNothingForPointersStillInUse() {
  byte memory[2 * (sizeof(AllocHeader) + sizeof(long))];
  Heap heap = createHeap(memory, sizeof(memory));

  void *allocation = allocFromHeap(&heap, sizeof(long));
  AllocHeader *header = allocation - sizeof(AllocHeader);

  ++header->used;

  deallocFromHeap(&heap, allocation);
  
  if (!header->used)
    return !!printf("Allocated space was marked 'free'.\n");

  if (header->end != allocation + sizeof(long))
    return !!printf("End pointer in allocation was improperly changed.\n");

  return 0;
}

test int shareAlloc_incrementsReferenceCount() {
  byte memory[sizeof(AllocHeader) + sizeof(long)];
  Heap heap = createHeap(memory, sizeof(memory));

  void *allocation = allocFromHeap(&heap, sizeof(long));
  AllocHeader *header = allocation - sizeof(AllocHeader);

  shareAlloc(allocation);

  if (header->used != 2)
    return !!printf("Reference count was not incremented.\n");

  return 0;
}

test int reallocFromHeap_doesNothingForZeroSize() {
  return !!printf("Test not yet implemented.\n");
}

test int reallocFromHeap_copiesToLocationWithMoreSpace() {
  byte memory[2 * sizeof(AllocHeader) + sizeof(int) + sizeof(long)];
  Heap heap = createHeap(memory, sizeof(memory));

  int *allocation = allocFromHeap(&heap, sizeof(int));
  *allocation = 0xef00ef00;
  shareAlloc(allocation);

  long *reallocation = reallocFromHeap(&heap, allocation, sizeof(long));

  if (reallocation == NULL)
    return !!printf("A null reference was returned.\n");

  if (*reallocation == *allocation)
    return !!printf("Data was not copied.\n");

  AllocHeader *allocHeader = allocation - sizeof(AllocHeader);
  AllocHeader *reallocHeader = reallocation - sizeof(AllocHeader);

  if (reallocHeader->used == allocHeader->used)
    return !!printf("Reference count was not copied.\n");

  return 0;
}

test int reallocFromHeap_copiesToLocationWithLessSpace() {
  byte memory[2 * sizeof(AllocHeader) + sizeof(int) + sizeof(short)];
  Heap heap = createHeap(memory, sizeof(memory));

  int *allocation = allocFromHeap(&heap, sizeof(int));
  *allocation = 0xef00ef00;
  shareAlloc(allocation);

  short *reallocation = reallocFromHeap(&heap, allocation, sizeof(short));

  if (reallocation == NULL)
    return !!printf("A null reference was returned.\n");

  if (*reallocation == *allocation)
    return !!printf("Data was not copied.\n");

  AllocHeader *allocHeader = allocation - sizeof(AllocHeader);
  AllocHeader *reallocHeader = reallocation - sizeof(AllocHeader);

  if (reallocHeader->used == allocHeader->used)
    return !!printf("Reference count was not copied.\n");

  return 0;
}
