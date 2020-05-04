#ifndef __MISC_INST__
#define __MISC_INST__

#include <stdio.h>

#include "../list.c"

void handleNoOpInst(TypedList *callStack) {
  printf("nop\n");
}

#endif
