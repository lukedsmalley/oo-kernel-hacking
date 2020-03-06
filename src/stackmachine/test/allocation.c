#include <stdio.h>

#include "../types.c"
#include "../allocation.c"

int test_endof() {
  byte value = 0;

  if (endof(value) != &value + sizeof(value)) {
    printf("endof() doesn't work.\n");
    return 1;
  }

  return 0;
}

int test_pushMemory() {
  byte from = 1;
  byte to = 0;
  byte *dest = &to;

  pushMemory(&dest, &to, endof(to));

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
