#ifndef __LIST__
#define __LIST__

#include "default-heap.c"

typedef struct {
  Heap *heap;
  void *items;
  ulong itemCount;
  ulong itemSize;
} List;

List createListOnDefaultHeap(ulong itemSize) {
  return (List) {
    .heap = &defaultHeap,
    .items = NULL,
    .itemCount = 0,
    .itemSize = itemSize
  };
}

boolean addToList(List *list, void *item, ulong itemSize) {
  /* This is really bad, but it's as safe as it can be. */
  /* ^ No it's not, doesn't even check for NULL on realloc */
  ulong size = list->itemCount * list->itemSize;
  void *reallocation = reallocFromHeap(list->heap, list->items, size + list->itemSize);
  if (reallocation == NULL) {
    return false;
  }
  list->items = reallocation;
  ulong minSize = list->itemSize < itemSize ? list->itemSize : itemSize;
  moveMemory(list->items + size, item, item + itemSize);
  ++list->itemCount;
  return true;
}

void *popFromList(List *list) {
  void *listItemEnd = list->items + (list->itemCount * list->itemSize);
  void *item = allocFromHeap(list->heap, list->itemSize);
  if (item == NULL) {
    return NULL;
  }
  moveMemory(item, listItemEnd - list->itemSize, listItemEnd);
  void *reallocation = reallocFromHeap(list->heap, list->items, (list->itemCount - 1) * list->itemSize);
  if (reallocation == NULL) {
    deallocFromHeap(list->heap, item);
    return NULL;
  }
  list->items = reallocation;
  --list->itemCount;
  return item;
}

void destroyList(List list) {
  deallocFromHeap(list.heap, list.items);
}

#endif
