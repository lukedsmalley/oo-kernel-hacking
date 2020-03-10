#include <stdio.h>

#include "../types.c"
#include "../heap.c"

test int getPointerFromReference_doesPointerDivision() {
  for (int i = 0; i < 32; i++) createObject();
  printf("%d\n", getPointerFromReference(4));
  return 1;
}
