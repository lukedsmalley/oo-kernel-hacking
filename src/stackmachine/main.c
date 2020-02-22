#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "instructions.c"



int main(const int argCount, const char **args) {
  if (argCount < 2) {
    printf("Expected a program file name.\n\n");
    printf("Usage: stackmachine <program-file>\n\n");
    return 1;
  }

  /* Load bytecode program from file */
  uint8_t *program;
  int programLength;
  FILE *file = fopen(args[1], "rb");
  fseek(file, 0, SEEK_END);
  programLength = ftell(file);
  rewind(file);
  program = malloc(programLength * sizeof(uint8_t));
  fread(program, programLength, 1, file);
  fclose(file);

  /* Set up stack and program pointers */
  stack_t stack;
  int instruction = 0;

  while (instruction < programLength) {
    (*instructionHandlers[program[instruction]])(&stack, program, &instruction);
  }

  free(program);
  printf("\n");
}
