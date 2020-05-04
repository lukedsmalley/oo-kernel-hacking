#ifndef __FUNCTION_CALL__
#define __FUNCTION_CALL__

#include "types.c"
#include "program.c"

typedef union {
  long longValue;
  double doubleValue;
  void* pointerValue;
} Untyped;

typedef struct {
  Untyped value;
  ULong type;
} Typed;

typedef_TypedList(Typed);

typedef struct {
  Function *function;
  TypedList arguments;
  TypedList stack;
  TypedList locals;
  ULong instPointer;
} FunctionCall;

FunctionCall createFunctionCall(Function *function, TypedList arguments) {
  return (FunctionCall) {
    function,
    arguments,
    .stack = createListOnDefaultHeap(sizeof(Untyped)),
    .locals = createListOnDefaultHeap(sizeof(Typed)),
    .instPointer = 0
  };
}

void destroyFunctionCall(FunctionCall call) {
  destroyList(call.arguments);
  destroyList(call.stack);
  destroyList(call.locals);
}

#endif
