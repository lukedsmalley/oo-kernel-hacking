#include <stdio.h>

#include "types.c"
#include "program.c"
#include "file-stream.c"
#include "runtime.c"
#include "default-heap.c"

Int main(Int argCount, const Char **args) {
  if (argCount < 2) {
    printf("Expected a program file name.\n\n");
    printf("Usage: stackmachine <program-file>\n\n");
    return 1;
  }

  createDefaultHeap();

  ByteStream in = openFileStream(args[1]);
  Program program;
  readProgramFromByteStream(&program, &in);
  closeFileStream(in);

  runProgram(program);

  destroyProgram(program);
}
