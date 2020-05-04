#ifndef __LIST__
#define __LIST__

#include "heap.c"

#define typedef_TypedList(Type) \
  typedef struct { \
    Heap *heap; \
    Type *items; \
    ULong count; \
  } Type##List; \
  \
  Boolean addTo##Type##List(Type##List *list, Type value) { \
    if (!growList(list->heap, (Any**)&list->items, &list->count, sizeof(Type))) { \
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
    }; \
  }

#define destroyList(list) \
  deallocFromHeap(list.heap, list.items)

Boolean growList(Heap *heap, Any **items, ULong *count, ULong itemSize) {
  Any *realloc = reallocFromHeap(heap, *items, *count * itemSize + itemSize);
  if (realloc == NULL) {
    return false;
  }
  *items = realloc;
  return true;
}

typedef_TypedList(Byte);
typedef_TypedList(ULong);

#endif
