#include <stdlib.h>
#include <string.h>

#include "types.c"

#ifndef __NICE_THINGS__
#define __NICE_THINGS__

Allocation *allocations = NULL;
ulong allocationCount = 1;

void *niceAlloc(ulong size) {
  // TODO: Check the return values of malloc/realloc
  if (allocations == NULL) {
    allocations = malloc(sizeof(Allocation));
    allocations[0].location = malloc(size);
    allocations[0].size = size;
    memset(allocations[0].location, 0, size);
    return allocations[0].location;
  } else {
    allocations = realloc(allocations, (++allocationCount) * sizeof(Allocation));
    allocations[allocationCount - 1].location = malloc(size);
    allocations[allocationCount - 1].size = size;
    memset(allocations[allocationCount - 1].location, 0, size);
    return allocations[allocationCount - 1].location;
  }
}

void niceFree(const void *allocation) {
  for (int i = 0; i < allocationCount; i++) {
    if (allocations[i].location == allocation) {
      for (int j = i; j < allocationCount - 1; j++) {
        allocations[j] = allocations[j + 1];
      }
      --allocationCount;
      allocations = realloc(allocations, allocationCount + sizeof(Allocation));
      return;
    }
  }
}

void *niceRealloc(void *allocation, ulong size) {
  for (ulong i = 0; i < allocationCount; i++) {
    if (allocations[i].location == allocation) {
      void *reallocation = niceAlloc(size);
      ulong minSize = size;
      if (size > allocations[i].size) {
        minSize = allocations[i].size;
      }
      memcpy(reallocation, allocations[i].location, minSize);
      niceFree(allocations[i].location);
      return reallocation;
    }
  }
  return niceAlloc(size);
}

void niceExit(int code) {
  for (int i = 0; i < allocationCount; i++) {
    if (allocations[i].location != NULL) {
      free(allocations[i].location);
    }
  }
  free(allocations);
  exit(code);
}

#endif
