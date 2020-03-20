#ifndef __RUNTIME__
#define __RUNTIME__

#include <stdio.h>
#include <stdlib.h>

#include "program.c"
#include "list.c"
#include "function-call.c"
#include "instructions/add.c"
#include "instructions/misc.c"

typedef struct {
  byte size;
  void (*handler)(List *callStack);
} Instruction;

Instruction instructions[] = {
  /* 0: nop */ { 0, handleNoOpInst },
  /* 1: add_long */ { 0, handleAddLongInst },
  /* 2: add_ulong */ { 0, handleAddLongInst },
  /* 3: add_double */ { 0, handleAddDoubleInst },
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
      printf("Reached the end of a function without returning.\n");
      exit(1);
    } else if (*op >= sizeof(instructions) / sizeof(Instruction)) {
      printf("An invalid instruction was encountered.\n");
      exit(1);
    } else {
      instructions[*op].handler(&callStack);
      call->instPointer += instructions[*op].size + 1;
    }
  }

  destroyList(callStack);
}

#endif
