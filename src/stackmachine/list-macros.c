#ifndef __LIST_MACROS__
#define __LIST_MACROS__

#include "types.c"
#include "default-heap.c"

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

#define addValueToList(listRef, value) \
  addToListFromMemory(listRef, sizeof(*listRef->items), &value, sizeof(value));

#define destroyList(name) \
  deallocFromDefaultHeap(name.items);

boolean addToListFromMemory(List(void) *list, ulong itemSize, void *value, ulong valueSize) {
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

#endif
