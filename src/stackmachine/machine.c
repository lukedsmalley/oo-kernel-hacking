#include "types.c"
#include "list.c"

typedef struct {
  ulong offset;
  ulong size;
} Function;

typedef struct {
  ulong offset;
  ulong size;
} Class;

typedef struct {
  List /* of Class */ classes;
  List /* of Function */ functions;
} Program;

void loadClasses(const Program *program, const Stream *stream) {
  ulong classTableSize = stream->next()
}

Program loadProgram(const Stream *stream) {
  Program program = {
    .classes = createList(sizeof(Class)),
    .functions = createList(sizeof(Function)) 
  };

  loadClasses(&program, stream);

  return program;
}

void destroyProgram(const Program *program) {
  destroyList(&program->classes);
  destroyList(&program->functions);
}
