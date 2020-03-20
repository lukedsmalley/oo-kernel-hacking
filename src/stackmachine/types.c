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
  ((void*)&var + sizeof(var))

typedef char boolean;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long ulong;

#endif
