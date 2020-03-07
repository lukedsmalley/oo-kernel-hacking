#include <stdio.h>

#include "../types.c"
#include "../allocation.c"

int test_endof() {
  byte value = 0;

  if ((byte*)endof(value) - &value != sizeof(value)) {
    printf("endof() does not return a pointer to the location after the " \
      "given memory location.\n");
    return 1;
  }

  return 0;
}

int test_pushMemory() {
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
