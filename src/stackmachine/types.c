#ifndef __TYPES__
#define __TYPES__

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

typedef char boolean;
typedef unsigned char byte;
typedef unsigned long ulong;
typedef char *string;

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

#endif
