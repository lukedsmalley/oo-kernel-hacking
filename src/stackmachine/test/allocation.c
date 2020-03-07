#include <stdio.h>

#include "../types.c"
#include "../allocation.c"

#define FUN_LONG 0xfedcba9876543210

test int test_endof() {
  byte value = 0;

  if ((byte*)endof(value) - &value != sizeof(value)) {
    printf("endof() does not return a pointer to the location after the " \
      "given memory location.\n");
    return 1;
  }

  return 0;
}

test int test_moveMemory() {
  long from = FUN_LONG;
  long to = 0;

  moveMemory(&to, &from, endof(from));

  if (to != from) {
    printf("Data was not copied.\n");
    return 1;
  }

  return 0;
}

test int test_pushMemory() {
  long from = 0xfedcba9876543210;
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

test int test_pushZeroBlock() {
  long to = 0xfedcba9876543210;
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

test int test_allocFromBuffer_writesHeaderAndReturnsZeroedBlock() {
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

test int test_allocFromBuffer_returnsNullForZeroSize() {
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

test int test_allocFromBuffer_returnsNullWhenOutOfSpace() {
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
}
