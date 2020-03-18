#ifndef __FUNCTION_CALL__
#define __FUNCTION_CALL__

#include "list.c"
#include "program.c"

typedef struct {
  Function *function;
  List arguments;
  List stack;
  ulong instPointer;
} FunctionCall;

FunctionCall createFunctionCall(Function *function, List arguments) {
  return (FunctionCall) {
    function,
    arguments,
    .stack = createListOnDefaultHeap(sizeof(void*)),
    .instPointer = 0
  };
}

void destroyFunctionCall(FunctionCall call) {
  destroyList(call.arguments);
  destroyList(call.stack);
}

#endif
