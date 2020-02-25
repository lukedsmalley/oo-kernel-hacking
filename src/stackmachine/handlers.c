#include <stdio.h>

#include "nice-things.c"

void handleNoOperationInstruction() {
  printf("nop\n");
}

void handleInvalidInstruction() {
  printf("An invalid instruction was encountered.\n");
  niceExit(1);
}
