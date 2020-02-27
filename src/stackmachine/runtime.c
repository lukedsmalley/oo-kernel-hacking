#ifndef __RUNTIME__
#define __RUNTIME__

#include "program.c"
#include "handlers.c"

typedef void (*InstructionHandler)();

typedef struct {
  byte size;
  InstructionHandler handler;
} Instruction;

Instruction instructions[] = {
  /* 0: nop */ { 0, handleNoOperationInstruction }
};

void runProgram(Program program) {
  ulong programPointer = 0;
  Function function = *((Function*)program.functions.items);
  while (programPointer < function.size) {
    byte opCode = function.body[programPointer];
    if (opCode >= sizeof(instructions) / sizeof(Instruction)) {
      handleInvalidInstruction();
    } else {
      instructions[opCode].handler();
      programPointer += instructions[opCode].size + 1;
    }
  }
}

#endif
