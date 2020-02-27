#ifndef __LIST__
#define __LIST__

#include "memory.c"

typedef struct {
  void *items;
  ulong itemCount;
  ulong itemSize;
} List;

List createList(ulong itemSize) {
  return (List){ NULL, 0, itemSize };
}

void addToList(List *list, const byte *item, ulong itemSize) {
  /* This is really bad, but it's as safe as it can be. */
  ulong index = list->itemCount * list->itemSize;
  list->items = grow(list->items, index + list->itemSize);
  ulong minSize = list->itemSize;
  if (itemSize < minSize) {
    minSize = itemSize;
  }
  for (int i = 0; i < minSize; i++) {
    ((byte*)list->items)[index + i] = item[i];
  }
}

void destroyList(List list) {
  dealloc(list.items);
}

#endif
