#ifndef __TYPES__
#define __TYPES__

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef test
#define test
#endif

#define endof(var) \
  ((Void*)&var + sizeof(var))

#define typedef_MaybeType(Type) \
  typedef struct { \
    Boolean hasValue; \
    Type value; \
  } Maybe##Type;

typedef void Void;
typedef void Any;
typedef char Char;
typedef char Boolean;
typedef unsigned char Byte;
typedef int Int;
typedef long Long;
typedef unsigned long ULong;

#endif
