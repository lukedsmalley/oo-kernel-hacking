#ifndef __RUNTIME__
#define __RUNTIME__

#include <stdio.h>
#include <stdlib.h>

#include "types.c"
#include "program.c"
#include "function-call.c"
#include "instructions/add.c"
#include "instructions/misc.c"

typedef struct {
  Byte size;
  Void (*handler)(FunctionList *callStack);
} Instruction;

Instruction instructions[] = {
  /* 0: nop */ { 0, handleNoOpInst },
  /* 1: add_long */ { 0, handleAddLongInst },
  /* 2: add_ulong */ { 0, handleAddLongInst },
  /* 3: add_double */ { 0, handleAddDoubleInst },
};

Void runProgram(Program program) {
  FunctionList callStack = createFunctionList(&defaultHeap);

  /* TODO: Pass remaining argv */
  FunctionCall entryCall = createFunctionCall(
    program.functions.items,
    createTypedList(&defaultHeap));
  addToList(&callStack, &entryCall, sizeof(entryCall));

  while (callStack.count > 0) {
    FunctionCall *call = (FunctionCall*)callStack.items + callStack.count - 1;
    Byte *op = call->function->instructions.items + call->instPointer;
    if (op >= call->function->instructions.items + call->function->instructions.count) {
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
