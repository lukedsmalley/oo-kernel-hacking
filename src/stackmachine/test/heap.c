#include <stdio.h>

#include "../types.c"
#include "../heap.c"

#define FUN_LONG 0xfedcba9876543210
#define FUN_LONG_BYTES 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe

test int endof_returnsEndPointer() {
  byte value = 0;

  if ((byte*)endof(value) - &value != sizeof(value)) {
    printf("endof() does not return a pointer to the location after the " \
      "given memory location.\n");
    return 1;
  }

  return 0;
}

test int moveMemory_copiesData() {
  long from = FUN_LONG;
  long to = 0;

  moveMemory(&to, &from, endof(from));

  if (to != from) {
    printf("Data was not copied.\n");
    return 1;
  }

  return 0;
}

test int pushMemory_copiesDataAndIncrementsPointer() {
  long from = FUN_LONG;
  long to = 0;
  long *dest = &to;

  pushMemory(&dest, &from, endof(from));

  if (to != from) {
    printf("Data was not copied.\n");
    return 1;
  }

  if (dest != endof(to)) {
    printf("Pointer passed by 'const void **dest' of pushMemory has not " \
      "been incremented.\n");
    return 1;
  }

  return 0;
}

test int pushZeroBlock_zeroesDataAndIncrementsPointer() {
  long to = FUN_LONG;
  long *dest = &to;

  pushZeroBlock(&dest, sizeof(to));

  if (to != 0) {
    printf("Data was not zeroed.\n");
    return 1;
  }

  if (dest != endof(to)) {
    printf("Pointer passed by 'const void **dest' of pushZeroBlock has " \
      "not been incremented.\n");
    return 1;
  }

  return 0;
}

test int createHeap_returnsHeap() {
  byte memory[sizeof(AllocHeader)];
  Heap heap = createHeap(memory, sizeof(memory));
  AllocHeader *header = heap.start;

  if (header->end != endof(memory)) {
    printf("Bad end pointer in placeholder header.\n");
    return 1;
  }

  if (header->used) {
    printf("Bad reference count in placeholder header.\n");
    return 1;
  }

  return 0;
}

test int allocFromHeap_addsToHeapAndReturnsPointer() {
  byte memory[2 * (sizeof(AllocHeader) + sizeof(long)) + sizeof(AllocHeader)];
  Heap heap = createHeap(memory, sizeof(memory));

  void* allocation = allocFromHeap(&heap, sizeof(long));

  if (allocation == NULL) {
    printf("A null reference was returned.\n");
    return 1;
  }

  AllocHeader *header = allocation - sizeof(AllocHeader);

  if (!header->used) {
    printf("Bad reference count in allocation header.\n");
    return 1;
  }

  if (header->end != sizeof(AllocHeader) + sizeof(long)) {
    printf("Bad end pointer in allocation header.\n");
    return 1;
  }

  if (*(long*)allocation != 0) {
    printf("Allocated memory wasn't zeroed.\n");
    return 1;
  }

  return 0;
}

/*test int test_allocFromBuffer_writesHeaderAndReturnsZeroedBlock() {
  byte memory[sizeof(AllocHeader) + sizeof(long)];
  AllocBuffer buffer = { memory, memory, endof(memory) };
  AllocHeader header = { sizeof(long), FUN_LONG };

  long *allocation = allocFromBuffer(&buffer, header);

  if (buffer.free != endof(memory)) {
    printf("The free space pointer wasn't properly incremented.\n");
    return 1;
  }

  if (allocation == NULL) {
    printf("No allocation was made though sufficient space was provided.\n");
    return 1;
  }

  if (allocation != memory + sizeof(header)) {
    printf("The returned pointer does not point to the allocated space.\n");
    return 1;
  }

  if (*allocation != 0) {
    printf("Data was not zeroed.\n");
    return 1;
  }

  AllocHeader *writtenHeader = (AllocHeader*)&memory;

  if (writtenHeader->size != header.size) {
    printf("Allocation header is malformed (bad size).\n");
    return 1;
  }

  if (writtenHeader->type != header.type) {
    printf("Allocation header is malformed (bad type).\n");
    return 1;
  }

  return 0;
}

//test int test_allocFromBuffer_returnsNullForZeroSize() {
  byte memory[sizeof(AllocHeader) + sizeof(long)];
  AllocBuffer buffer = { memory, memory, endof(memory) };
  AllocHeader header = { 0, FUN_LONG };

  long *allocation = allocFromBuffer(&buffer, header);

  if (buffer.free != buffer.start) {
    printf("The free space pointer was moved.\n");
    return 1;
  }

  if (allocation != NULL) {
    printf("A pointer was returned.\n");
    return 1;
  }

  return 0;
}

//test int test_allocFromBuffer_returnsNullWhenOutOfSpace() {
  byte memory[sizeof(AllocHeader) + sizeof(int)];
  AllocBuffer buffer = { memory, memory, endof(memory) };
  AllocHeader header = { sizeof(long), FUN_LONG };

  long *allocation = allocFromBuffer(&buffer, header);

  if (buffer.free != buffer.start) {
    printf("The free space pointer was moved.\n");
    return 1;
  }

  if (allocation != NULL) {
    printf("A pointer was returned.\n");
    return 1;
  }

  return 0;
}*/

/*test int test_deallocFromBuffer_shiftsRemainder() {
  byte memory[] = {*/
    /* Allocation Entry */
    //8, 0, 0, 0, 0, 0, 0, 0, /* size */
    //0, 0, 0, 0, 0, 0, 0, 0, /* type */
    //FUN_LONG_BYTES,         /* data (long) */
    /* Remainder Entry */
    /*8, 0, 0, 0, 0, 0, 0, 0,
    FUN_LONG_BYTES,
    FUN_LONG_BYTES
  };
  AllocBuffer buffer = { memory, endof(memory), endof(memory) };
  long *allocation = &memory[sizeof(AllocHeader)];

  deallocFromBuffer(&buffer, allocation);

  if (buffer.free != &memory[sizeof(AllocHeader) + 8]) {
    printf("The free space pointer was not moved backward.\n");
    return 1;
  }

  if (((AllocHeader*)memory)->type != FUN_LONG ||
      *allocation != FUN_LONG) {
    printf("Remainder was not correctly shifted backward.\n");
    return 1;
  }

  return 0;
}*/

/*test int test_deallocFromBuffer_doesNothingForNullPointer() {
  byte memory[0];
  AllocBuffer buffer = { memory, memory, memory };
  
  deallocFromBuffer(&buffer, NULL);

  if (buffer.free != memory) {
    printf("The free space pointer was moved.\n");
    return 1;
  }

  return 0;
}*/
