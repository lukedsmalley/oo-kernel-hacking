#ifndef __PROGRAM__
#define __PROGRAM__

#include <stdlib.h>

#include "types.c"
#include "default-heap.c"

typedef struct {
  ULong id;
  ByteList name;
  ULong type;
} Field;

typedef_TypedList(Field);

typedef struct {
  ULong id;
  ByteList name;
  ULongList superclasses;
  FieldList fields;
  FieldList methods;
} Class;

typedef_TypedList(Class);

typedef struct {
  ULong id;
  ByteList name;
  ULong type;
  FieldList parameters;
  ByteList instructions;
} Function;

typedef_TypedList(Function);

typedef struct {
  ClassList classes;
  FunctionList functions;
} Program;

Boolean readULongFromByteStream(ULong *value, ByteStream *stream) {
  *value = 0;
  for (int i = 0; i < 8; i++) {
    if (!readStream(stream)) {
      return false;
    }
    *value <<= 8;
    *value += stream->value;
  }
  return true;
}

Boolean readFieldFromByteStream(Field *field, ByteStream *stream) {
  field->name = createByteList(&defaultHeap);
  ULong nameLength;
  if (!readULongFromByteStream(&nameLength, stream)) {
    printf("Incomplete name length field in field %d\n", field->id);
    return false;
  }
  for (ULong i = 0; i < nameLength; i++) {
    if (!readStream(stream)) {
      printf("Incomplete name field in field %d\n", field->id);
      return false;
    }
    addToByteList(&field->name, stream->value);
  }

  if (!readULongFromByteStream(&field->type, stream)) {
    printf("Incomplete type field in field %d\n", field->id);
    return false;
  }

  return true;
}

Boolean readClassFromByteStream(Class *class, ByteStream *stream) {
  class->name = createByteList(&defaultHeap);
  ULong nameLength;
  if (!readULongFromByteStream(&nameLength, stream)) {
    printf("Incomplete name length field in class %d\n", class->id);
    return false;
  }
  for (ULong i = 0; i < nameLength; i++) {
    if (!readStream(stream)) {
      printf("Incomplete name field in class %d\n", class->id);
      return false;
    }
    addToByteList(&class->name, stream->value);
  }

  class->superclasses = createULongList(&defaultHeap);
  ULong superclassCount;
  if (!readULongFromByteStream(&superclassCount, stream)) {
    printf("Incomplete superclass count field in class %d\n", class->id);
    return false;
  }
  for (ULong i = 0; i < superclassCount; i++) {
    ULong superclassID;
    if (!readULongFromByteStream(&superclassID, stream)) {
      printf("Incomplete superclass list field in class %d\n", class->id);
      return false;
    }
    addToULongList(&class->superclasses, superclassID);
  }

  class->fields = createFieldList(&defaultHeap);
  ULong fieldCount;
  if (!readULongFromByteStream(&fieldCount, stream)) {
    printf("Incomplete field count field in class %d\n", class->id);
    return false;
  }
  for (ULong id = 0; id < fieldCount; id++) {
    Field field = { id };
    if (!readFieldFromByteStream(&field, stream)) {
      printf("Incomplete field in class %d\n", class->id);
      return false;
    }
    addToFieldList(&class->fields, field);
  }

  class->methods = createFieldList(&defaultHeap);
  ULong methodCount;
  if (!readULongFromByteStream(&methodCount, stream)) {
    printf("Incomplete method count field in class %d\n", class->id);
    return false;
  }
  for (ULong id = 0; id < methodCount; id++) {
    Field method = { id };
    if (!readFieldFromByteStream(&method, stream)) {
      printf("Incomplete method in class %d\n", class->id);
      return false;
    }
    addToFieldList(&class->methods, method);
  }

  return true;
}

Boolean readFunctionFromByteStream(Function *function, ByteStream *stream) {
  function->name = createByteList(&defaultHeap);
  ULong nameLength;
  if (!readULongFromByteStream(&nameLength, stream)) {
    printf("Incomplete name length field in function %d\n", function->id);
    return false;
  }
  for (ULong i = 0; i < nameLength; i++) {
    if (!readStream(stream)) {
      printf("Incomplete name field in function %d\n", function->id);
      return false;
    }
    addToByteList(&function->name, stream->value);
  }

  if (!readULongFromByteStream(&function->type, stream)) {
    printf("Incomplete type field in function %d\n", function->id);
    return false;
  }

  function->parameters = createFieldList(&defaultHeap);
  ULong parameterCount;
  if (!readULongFromByteStream(&parameterCount, stream)) {
    printf("Incomplete parameter count field in function %d\n", function->id);
    return false;
  }
  for (ULong id = 0; id < parameterCount; id++) {
    Field parameter = { id };
    if (!readFieldFromByteStream(&parameter, stream)) {
      printf("Incomplete parameter in function %d\n", function->id);
      return false;
    }
    addToFieldList(&function->parameters, parameter);
  }

  function->instructions = createByteList(&defaultHeap);
  ULong instructionCount;
  if (!readULongFromByteStream(&instructionCount, stream)) {
    printf("Incomplete instruction count field in function %d\n", function->id);
    return false;
  }
  for (ULong i = 0; i < instructionCount; i++) {
    if (!readStream(stream)) {
      printf("Incomplete instruction body field in function %d\n", function->id);
      return false;
    }
    addToByteList(&function->instructions, stream->value);
  }

  return true;
}

Boolean readProgramFromByteStream(Program *program, ByteStream *stream) {
  program->classes = createClassList(&defaultHeap);
  ULong classCount;
  if (!readULongFromByteStream(&classCount, stream)) {
    printf("Incomplete class count field\n");
    return false;
  }
  for (ULong id = 0; id < classCount; id++) {
    Class class = { id };
    if (!readClass(&class, stream)) {
      return false;
    }
    addToClassList(&program->classes, class);
  }
  
  program->functions = createFunctionList(&defaultHeap);
  ULong functionCount;
  if (!readULongFromByteStream(&functionCount, stream)) {
    printf("Incomplete function count field\n");
    return false;
  }
  for (ULong id = 0; id < functionCount; id++) {
    Function function = { id };
    if (!readFunction(&function, stream)) {
      return false;
    }
    addToFunctionList(&program->functions, function);
  }
  
  return true;
}

void destroyProgram(Program program) {
  destroyList(program.classes);
  destroyList(program.functions);
}

#endif
