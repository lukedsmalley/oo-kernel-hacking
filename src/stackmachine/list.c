#ifndef __LIST__
#define __LIST__

#include "heap.c"

typedef struct {
  Heap heap;
  void *items;
  ulong itemCount;
  ulong itemSize;
} List;

List createList(Heap heap, ulong itemSize) {
  return (List) {
    .heap = heap,
    .items = NULL,
    .itemCount = 0,
    .itemSize = itemSize
  };
}

void addToList(List *list, const byte *item, ulong itemSize) {
  /* This is really bad, but it's as safe as it can be. */
  /* ^ No it's not, doesn't even check for NULL on realloc */
  ulong index = list->itemCount * list->itemSize;
  list->items = reallocFromHeap(&list->heap, list->items, index + list->itemSize);
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
