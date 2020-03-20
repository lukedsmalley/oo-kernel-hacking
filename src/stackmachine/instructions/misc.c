#ifndef __MISC_INST__
#define __MISC_INST__

#include <stdio.h>

#include "../list.c"

void handleNoOpInst(List *callStack) {
  printf("nop\n");
}

#endif
