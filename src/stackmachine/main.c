#include <stdio.h>
#include <stdlib.h>

#include "types.c"
#include "handlers.c"
#include "nice-things.c"

Instruction instructions[] = {
  /* 0: nop */ { 0, handleNoOperationInstruction }
};

long readLong(const byte *buffer, int position) {
  return buffer[position] + ((long)buffer[++position] << 8) +
    ((long)buffer[++position] << 16) + ((long)buffer[++position] << 24) +
    ((long)buffer[++position] << 32) + ((long)buffer[++position] << 40) +
    ((long)buffer[++position] << 48) + ((long)buffer[++position] << 56);
}

Program loadPogram(const char *filename) {
  // TODO: Check niceAlloc return value
  Program program;
  FILE *file = fopen(filename, "rb");
  fseek(file, 0, SEEK_END);
  program.length = ftell(file);
  rewind(file);
  program.data = niceAlloc(program.length * sizeof(byte));
  fread(program.data, program.length, 1, file);
  fclose(file);

  if (program.length < 8) {
    printf("The class table header is too short.\n");
    niceExit(1);
  }

  ulong functionTableLocation = readLong(program.data, 0) + 8;

  if (program.length < functionTableLocation) {
    printf("The program is shorter than the class table header says it is.\n");
    niceExit(1);
  }

  if (program.length < functionTableLocation + 8) {
    printf("The function table header is too short.\n");
    niceExit(1);
  }

  ulong endOfFile = functionTableLocation +
    readLong(program.data, functionTableLocation) + 8;

  if (program.length < endOfFile) {
    printf("The program is shorter than the function table header says it is.\n");
    niceExit(1);
  }

  int functionLocation = functionTableLocation + 8;
  while (functionLocation < endOfFile) {
    program.functions = niceRealloc(program.functions, (++program.functionCount) * sizeof(Function));
    program.functions[program.functionCount - 1].location = functionLocation;
    ulong functionLength = readLong(program.data, functionLocation);
    program.functions[program.functionCount - 1].length = functionLength;
    functionLocation += functionLength + 8;
  }
  
  return program;
}

int main(int argCount, const char **args) {
  if (argCount < 2) {
    printf("Expected a program file name.\n\n");
    printf("Usage: stackmachine <program-file>\n\n");
    return 1;
  }

  Program program = loadPogram(args[1]);
  int programPointer = 0;

  while (programPointer < program.length) {
    byte opCode = program.data[program.functions[0].location + 8 + programPointer];
    if (opCode >= sizeof(instructions) / sizeof(Instruction)) {
      handleInvalidInstruction();
    } else {
      instructions[opCode].handler();
      programPointer += instructions[opCode].length + 1;
    }
  }

  niceExit(0);
}
