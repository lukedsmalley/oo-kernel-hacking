#ifndef __HANDLERS__
#define __HANDLERS__

#include <stdio.h>

#include "function-call.c"
#include "list.c"

void handleNoOpInst(List *callStack) {
  printf("nop\n");
}

void handleAddUncheckedInst(List *callStack) {
  printf("add_unchecked\n");
  FunctionCall *call = (FunctionCall*)callStack->items + callStack->itemCount - 1;
  
}

void handleFunctionWithoutReturn() {
  printf("Reached the end of a function without returning.\n");
  exit(1);
}

void handleInvalidInst() {
  printf("An invalid instruction was encountered.\n");
  exit(1);
}

#endif
