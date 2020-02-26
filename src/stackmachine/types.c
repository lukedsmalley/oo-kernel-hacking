#ifndef __TYPES__
#define __TYPES__

typedef char boolean;
typedef unsigned char byte;
typedef unsigned long ulong;

typedef struct {
  void *location;
  ulong size;
} Allocation;

typedef void (*InstructionHandler)();

typedef struct {
  byte length;
  InstructionHandler handler;
} Instruction;

/*typedef struct {
  byte *data;
  ulong length;
  Function *functions;
  ulong functionCount;
} Program;*/

typedef struct {
  byte (*next)();
  boolean (*hasNext)();
} Stream;

#endif
