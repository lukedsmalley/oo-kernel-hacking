#include "memory.c"

typedef struct {
  void *buffer;
  ulong itemCount;
  ulong itemSize;
} List;

List createList(ulong itemSize) {
  return (List){ NULL, 0, itemSize };
}

void addToList(List *list, const void *item, ulong itemSize) {
  /* This is really bad, but it's as safe as it can be. */
  ulong index = list->itemCount * list->itemSize;
  list->buffer = reallocate(list->buffer, index + list->itemSize);
  ulong minSize = list->itemSize;
  if (itemSize < minSize) {
    minSize = itemSize;
  }
  for (int i = 0; i < minSize; i++) {
    ((byte*)list->buffer)[index + i] = ((byte*)item)[i];
  }
}

void destroyList(const List *list) {
  deallocate(list->buffer);
}
