#include <stdio.h>

#include "types.c"
#include "program.c"
#include "file-stream.c"
#include "stream.c"
#include "runtime.c"

/*Program_ loadProgram_(const char *filename) {
  // TODO: Check niceAlloc return value
  Program_ program;
  FILE *file = fopen(filename, "rb");
  fseek(file, 0, SEEK_END);
  program.length = ftell(file);
  rewind(file);
  program.data = niceAlloc(program.length * sizeof(byte));
  fread(program.data, program.length, 1, file);
  fclose(file);

  ulong classTableSize = readLong(program.data, program.length, 0,
    "The class table header is too short.\n",
    "The class table header is too short.\n");

  ulong functionTableLocation = classTableSize + 8;
  ulong functionTableSize = readLong(program.data, program.length, functionTableLocation,
    "The program is shorter than the class table header says it is.\n",
    "The function table header is too short.\n");

  ulong eof = functionTableLocation + 8 + functionTableSize;
  if (program.length < eof) {
    printf("The program is shorter than the function table header says it is.\n");
    niceExit(1);
  }

  int functionLocation = functionTableLocation + 8;
  while (functionLocation < eof) {
    program.functions = niceRealloc(program.functions, (++program.functionCount) * sizeof(Function_));
    Function_ *function = &program.functions[program.functionCount - 1];

    function->offset = functionLocation;
    function->size = readLong(program.data, program.length, functionLocation,
      "The program is shorter than the function headers say it is.\n",
      "");
    
    functionLocation += function->size + 8;
  }
  
  return program;
}*/

int main(int argCount, const char **args) {
  if (argCount < 2) {
    printf("Expected a program file name.\n\n");
    printf("Usage: stackmachine <program-file>\n\n");
    return 1;
  }

  Stream in = openFileStream(args[1]);
  printf("0\n");
  Program program = loadProgram(in);
  printf("1\n");
  runProgram(program);
  printf("2\n");
  destroyProgram(program);
}
