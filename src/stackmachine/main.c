#include <stdio.h>

#include "types.c"
#include "program.c"
#include "file-stream.c"
#include "stream.c"
#include "runtime.c"
#include "heap.c"

#define MEMORY_SIZE 4096

int main(int argCount, const char **args) {
  if (argCount < 2) {
    printf("Expected a program file name.\n\n");
    printf("Usage: stackmachine <program-file>\n\n");
    return 1;
  }

  byte memory[MEMORY_SIZE];
  Heap heap = createHeap(bufferof(memory));

  Stream in = openFileStream(args[1]);
  Program program = loadProgram(heap, in);
  closeFileStream(in);

  runProgram(program);

  destroyProgram(program);
}
