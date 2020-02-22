#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "types.c"

typedef void (*instruction_handler_t)(stack_t*, uint8_t[], uint8_t*);

const instruction_handler_t *instructionHandlers[256];

void handleNoOperationInstruction(const stack_t *stack, uint8_t program[], uint8_t *instruction) {
  printf("nop\n");
}

void handleInvalidInstruction(const stack_t *stack, uint8_t program[], uint8_t *instruction) {
  printf("Invalid instruction\n");
}

void createInstructionHandlerJumpTable() {
  for (int i = 256; i--;) {
    instructionHandlers[i] = &handleInvalidInstruction;
  }
  instructionHandlers[0] = &handleNoOperationInstruction;
}
