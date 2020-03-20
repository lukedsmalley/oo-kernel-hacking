#ifndef __LIST_MACROS__
#define __LIST_MACROS__

#include "types.c"
#include "default-heap.c"

#define ARRAY_PARAM(type, name, size) \
  type name[size]; \
  ulong name##_itemCount;

#define ARRAY_VAR(type, name, count) \
  type name[size]; \
  ulong name##itemCount = count;

#define ARRAY_VAR_OF(type, name, values) \
  type name[] = values; \
  ulong name##_itemCount = sizeof(name) / sizeof(type)

#define List(type) \
  struct { \
    type *items; \
    ulong count; \
  }

#define emptyList() \
  { \
    .items = NULL, \
    .count = 0 \
  }

#define emptyListOf(type) \
  (List(type)) { \
    .items = NULL, \
    .count = 0 \
  }

#define LIST_PARAM(type, name) \
  type *name; \
  ulong name##_itemCount
//ulong name##_itemSize; \

#define INIT_LIST_PARAM(name) \
  .##name = NULL, \
  .##name##_itemCount = 0

#define LIST_VAR(type, name) \
  type *name = NULL; \
  ulong name##_itemSize = sizeof(type); \
  ulong name##_itemCount = 0

#define destroyList(name) \
  deallocFromDefaultHeap(name);

boolean addMemoryToList(List(void) *list, ulong itemSize, void *value, ulong valueSize) {
  ulong size = itemSize * list->count;
  void *reallocation = reallocFromDefaultHeap(list->items, size + itemSize);
  if (reallocation == NULL) {
    return false;
  }
  list->items = reallocation;
  ulong minSize = valueSize < itemSize ? valueSize : itemSize;
  moveMemory(list->items + size, value, value + valueSize);
  ++list->count;
  return true;
}

#define addValueToList(listPointer, value) \
  addMemoryToList(listPointer, sizeof(*listPointer->items), &value, sizeof(value));

#endif
