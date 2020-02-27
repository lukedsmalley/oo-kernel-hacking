#include <stdio.h>
#include <stdlib.h>

#include "types.c"
#include "handlers.c"
#include "machine.c"

Instruction instructions[] = {
  /* 0: nop */ { 0, handleNoOperationInstruction }
};

int main(int argCount, const char **args) {
  if (argCount < 2) {
    printf("Expected a program file name.\n\n");
    printf("Usage: stackmachine <program-file>\n\n");
    return 1;
  }

  Program_ program = loadProgram_(args[1]);
  int programPointer = 0;

  while (programPointer < program.length) {
    byte opCode = program.data[program.functions[0].offset + 8 + programPointer];
    if (opCode >= sizeof(instructions) / sizeof(Instruction)) {
      handleInvalidInstruction();
    } else {
      instructions[opCode].handler();
      programPointer += instructions[opCode].length + 1;
    }
  }

  niceExit(0);
}
