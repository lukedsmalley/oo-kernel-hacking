#include "types.c"
#include "list.c"
#include "stream.c"

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

char *loadClasses(const Program *program, const Stream *in) {
  long *classTableLength = readLongFromStream(in);
  if (classTableLength == NULL) {
    free(classTableLength);
    return String("The class table header is too short.");
  }

  for (ulong i = 0; i < *classTableLength; i++) {
    if (!streamHasBytes(in)) {
      return String("The class table is shorter than its header says it is.");
    }
    readByteFromStream(in);
  }

  free(classTableLength);
  return NULL;
}

string loadFunctions(const Program *program, const Stream *in) {
  long *functionCount = readLongFromStream(in);
  if (functionCount == NULL) {
    free(functionCount);
    return String("Unexpected EOF in function table header");
  }

  for (ulong i = 0; i < *functionCount; i++) {
    long *functionLength = readLongFromStream(in);
    if (functionLength == NULL) {
      free(functionCount);
      free(functionLength);
      return String("Unexpected EOF in function body");
    }

    addToList(program->functions, 
  }
  ulong functionOffset
  for (ulong i = ; i < *classTableLength; i++) {
    if (!streamHasBytes(in)) {
      return allocString("The class table is shorter than its header says it is.");
    }
    readByteFromStream(in);
  }

  return NULL;
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
