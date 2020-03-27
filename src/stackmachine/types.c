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

#define typedef_TypedStream(Type) \
  typedef struct { \
    Any *handle; \
    Boolean hasValue; \
    Type value; \
    Boolean (*reader)(Type##Stream *stream); \
  } Type##Stream; \

#define readStream(streamPtr) \
  streamPtr->reader(streamPtr)

#define typedef_TypedList(Type) \
  typedef struct { \
    Heap *heap; \
    Type *items; \
    ULong count; \
    Boolean (*appender)(Type##List *list, Type value) \
  } Type##List; \
  \
  Boolean addTo##Type##List(Type##List *list, Type value) { \
    if (!growList(list->heap, &list->items, &list->count, sizeof(Type))) { \
      return false; \
    } \
    list->items[list->count++] = value; \
    return true; \
  } \
  \
  Type##List create##Type##List(Heap *heap) { \
    return (Type##List) { \
      heap, \
      .items = NULL, \
      .count = 0, \
      .appender = addTo##Type##List \
    }; \
  }

#define destroyList(list) \
  deallocFromHeap(list.heap, list.items)

typedef void Void;
typedef void Any;
typedef char Char;
typedef char Boolean;
typedef unsigned char Byte;
typedef int Int;
typedef long Long;
typedef unsigned long ULong;

#include "heap.c"

Boolean growList(Heap *heap, Any **items, ULong *count, ULong itemSize) {
  Any *realloc = reallocFromHeap(heap, *items, *count * itemSize + itemSize);
  if (realloc == NULL) {
    return false;
  }
  *items = realloc;
  return true;
}

typedef_TypedStream(Byte);

typedef_TypedList(Byte);
typedef_TypedList(ULong);

#endif
