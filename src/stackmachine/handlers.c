#ifndef __HANDLERS__
#define __HANDLERS__

#include <stdio.h>

void handleNoOperationInstruction() {
  printf("nop\n");
}

void handleInvalidInstruction() {
  printf("An invalid instruction was encountered.\n");
  exit(1);
}

#endif
