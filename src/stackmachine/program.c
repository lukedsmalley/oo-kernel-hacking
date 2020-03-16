#ifndef __PROGRAM__
#define __PROGRAM__

#include <stdlib.h>

#include "types.c"
#include "list.c"
#include "stream.c"
#include "heap.c"

typedef struct {
  byte *body;
  ulong size;
} Function;

typedef struct {
  List /* of Function */ functions;
} Program;

void loadFunctions(const Program *program, Stream in) {
  StreamItem functionCount = readStreamQWord(in);
  if (functionCount.done) {
    printf("The function table header is too short.\n");
    exit(0);
  }

  for (ulong i = 0; i < functionCount.value; i++) {
    StreamItem size = readStreamQWord(in);
    if (size.done) {
      printf("The program is shorter than the function table header says it is.\n");
      exit(0);
    }

    StreamItemList body = readStreamItems(in, size.value);
    if (body.done) {
      printf("The program is shorter than the function table header says it is.\n");
      exit(0);
    }

    addToList(&program->functions, &(Function) {
      .body = body.values.items,
      .size = body.values.itemCount
    }, sizeof(Function));
  }
}

Program loadProgram(Heap heap, Stream in) {
  Program program = {
    .functions = createList(heap, sizeof(Function))
  };

  loadFunctions(&program, in);

  return program;
}

void destroyProgram(Program program) {
  destroyList(program.functions);
}

#endif
