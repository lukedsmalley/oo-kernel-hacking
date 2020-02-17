#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 4096

int main() {
  unsigned char program[] = { 0 };
  int programLength = sizeof(program) / sizeof(unsigned char);

  unsigned char stack[STACK_SIZE];
  int stackPointer = 0;

  for (int i = 0; i < STACK_SIZE; i++) {
    stack[i] = 0;
  }

  int programPointer = 0;

  while (programPointer < programLength) {
    switch(program[programPointer]) {
      default:
        printf("Unknown instruction %d\n", program[programPointer]);
        return 1;
    }
  }
}
