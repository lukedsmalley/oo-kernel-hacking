#ifndef __STREAM__
#define __STREAM__

#include "types.c"

#define typedef_TypedStream(Type) \
  typedef struct { \
    Any *handle; \
    Boolean hasValue; \
    Type value; \
    Boolean (*reader)(Any *handle); \
  } Type##Stream

#define readStream(streamPtr) \
  streamPtr->reader(streamPtr)

typedef_TypedStream(Byte);

#endif
