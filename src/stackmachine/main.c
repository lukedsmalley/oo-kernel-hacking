#include <stdio.h>

#include "types.c"
#include "program.c"
#include "file-stream.c"
#include "stream.c"
#include "runtime.c"
#include "default-heap.c"

int main(int argCount, const char **args) {
  if (argCount < 2) {
    printf("Expected a program file name.\n\n");
    printf("Usage: stackmachine <program-file>\n\n");
    return 1;
  }

  createDefaultHeap();

  Stream in = openFileStream(args[1]);
  Program program = loadProgram(in);
  closeFileStream(in);

  runProgram(program);

  destroyProgram(program);
}
