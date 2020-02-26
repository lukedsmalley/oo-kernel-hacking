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

typedef struct {
  ulong offset;
  ulong size;
} Function_;

typedef struct {
  byte *data;
  ulong length;
  Function_ *functions;
  ulong functionCount;
} Program_;

typedef struct {
  void *handle;
  byte (*next)(void*);
  boolean (*hasNext)(void*);
} Stream;

#endif
