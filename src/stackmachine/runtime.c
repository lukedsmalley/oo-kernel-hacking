#ifndef __RUNTIME__
#define __RUNTIME__

#include "program.c"
#include "handlers.c"
#include "list.c"
#include "function-call.c"

typedef struct {
  byte size;
  void (*handler)(List *callStack);
} Instruction;

Instruction instructions[] = {
  /* 0: nop */ { 0, handleNoOpInst },
  /* 1: add_unchecked */ { 0, handleAddUncheckedInst },
};

void runProgram(Program program) {
  List callStack = createListOnDefaultHeap(sizeof(FunctionCall));

  /* TODO: Pass remaining argv */
  FunctionCall entryCall = createFunctionCall(
    program.functions.items,
    createListOnDefaultHeap(sizeof(void*)));
  addToList(&callStack, &entryCall, sizeof(entryCall));

  while (callStack.itemCount > 0) {
    FunctionCall *call = (FunctionCall*)callStack.items + callStack.itemCount - 1;
    byte *op = call->function->body + call->instPointer;
    if (op >= call->function->body + call->function->size) {
      handleFunctionWithoutReturn();
    } else if (*op >= sizeof(instructions) / sizeof(Instruction)) {
      handleInvalidInst();
    } else {
      instructions[*op].handler(&callStack);
      call->instPointer += instructions[*op].size + 1;
    }
  }

  destroyList(callStack);
}

#endif
