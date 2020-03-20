#ifndef __ADD_INST__
#define __ADD_INST__

#include <stdio.h>

#include "../list.c"
#include "../function-call.c"

void handleAddLongInst(List *callStack) {
  printf("add_long\n");
  FunctionCall *call = (FunctionCall*)callStack->items + callStack->itemCount - 1;
  /* If popFromList returns NULL, then mistakes were made long before we got here */
  Untyped *a = popFromList(&call->stack);
  Untyped *b = popFromList(&call->stack);
  Untyped sum = { .longValue = a->longValue + b->longValue };
  deallocFromHeap(call->stack.heap, a);
  deallocFromHeap(call->stack.heap, b);
  addToList(&call->stack, &sum, sizeof(sum));
}

void handleAddDoubleInst(List *callStack) {
  printf("add_double\n");
  FunctionCall *call = (FunctionCall*)callStack->items + callStack->itemCount - 1;
  Untyped *a = popFromList(&call->stack);
  Untyped *b = popFromList(&call->stack);
  Untyped sum = { .doubleValue = a->doubleValue + b->doubleValue };
  deallocFromHeap(call->stack.heap, a);
  deallocFromHeap(call->stack.heap, b);
  addToList(&call->stack, &sum, sizeof(sum));
}

void handleAddLongCheckedInst(List *callStack) {
  printf("add_long_checked\n");
  FunctionCall *call = (FunctionCall*)callStack->items + callStack->itemCount - 1;
  Untyped *a = popFromList(&call->stack);
  Untyped *b = popFromList(&call->stack);
  Untyped sum = { .doubleValue = a->doubleValue + b->doubleValue };
  deallocFromHeap(call->stack.heap, a);
  deallocFromHeap(call->stack.heap, b);
  addToList(&call->stack, &sum, sizeof(sum));
}

#endif
