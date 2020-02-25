#ifndef __TYPES__
#define __TYPES__

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

typedef struct {
  ulong length;
  ulong location;
} Function;

typedef struct {
  byte *data;
  ulong length;
  Function *functions;
  ulong functionCount;
} Program;

#endif
