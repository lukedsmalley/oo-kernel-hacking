#ifndef __PROGRAM__
#define __PROGRAM__

#include <stdlib.h>

#include "types.c"
#include "stream.c"
#include "heap.c"
#include "list-macros.c"

typedef struct {
  ulong id;
  List(byte) name;
  ulong type;
} Field;

typedef struct {
  ulong id;
  List(byte) name;
  ulong type;
  List(Field) parameters;
  List(byte) instructions;
} Function;

typedef struct {
  ulong id;
  List(byte) name;
  List(Field) fields;
  List(Function) methods;
  List(ulong) superclasses;
} Class;

typedef struct {
  List(Class) classes;
  List(Function) functions;
} Program;

void loadFunctions(Program *program, Stream in) {
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

    List body = readStreamItems(in, size.value);
    if (body.itemCount < size.value) {
      printf("The program is shorter than the function table header says it is.\n");
      exit(0);
    }

    Function function = {
      .body = body.items,
      .size = body.itemCount
    };
    addToList(&program->functions, &function, sizeof(function));
  }
}

boolean readByteIntoList(List(byte) *list, ulong id, Stream stream) {
  byte value;
  if (!readStream(&value, stream)) return false;
  return addValueToList(list, value);
}

boolean readField(List(Field) *list, ulong id, Stream stream) {
  Field field = { id };

  field.name = emptyListOf(byte);
  if (!readListFromStream(&name, readByteIntoList, stream, readStream))
    return false;

  if (!readStreamQWord(field.type, stream))
    return false;

  return addValueToList(list, field);
}

boolean readClass(List(Class) *classes, ulong id, Stream stream) {
  Class class = { id };

  if (!)

  return addValueToList(classes, class);
}

boolean readFunction(List(Function) *functions, ulong index, Stream stream) {
  
}

boolean readProgram(Program *program, Stream stream) {
  if (!readListFromStream(&program->classes, readClass, stream, readStreamQWord))
    return false;

  if (!readListFromStream(&program->functions, readFunction, stream, readStreamQWord))
    return false;

  return true;
}

void destroyProgram(Program program) {
  destroyList(program.classes);
  destroyList(program.functions);
}

#endif
