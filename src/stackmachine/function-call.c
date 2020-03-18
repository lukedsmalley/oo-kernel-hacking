#ifndef __FUNCTION_CALL__
#define __FUNCTION_CALL__

#include "list.c"
#include "program.c"

typedef struct {
  void *item;
  ulong type;
} Box;

typedef struct {
  Function *function;
  List arguments;
  List stack;
  List locals;
  ulong instPointer;
} FunctionCall;

FunctionCall createFunctionCall(Function *function, List arguments) {
  return (FunctionCall) {
    function,
    arguments,
    .stack = createListOnDefaultHeap(sizeof(Box)),
    .locals = createListOnDefaultHeap(sizeof(void*)),
    .instPointer = 0
  };
}

void destroyFunctionCall(FunctionCall call) {
  destroyList(call.arguments);
  destroyList(call.stack);
  destroyList(call.locals);
}

#endif
