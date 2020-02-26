#include "memory.h"

#ifndef __RESULT__
#define __RESULT__

#define Result(type)  type *

Result(char) Error(char *message) {
  char *end = 0;
  while (*end != NULL) {
    end++;
  }
  char *error = allocate(end - message + 1, Error);
  for (char *i = message; i <= end; i++) {
    *i = *message;
  }
  return error;
}

Result(char) createEmptyError() {
  return allocate(sizeof(char), Error);
}

Result(char) rewrapError(char *error, char *message) {
  char *newError = Error(message);
  deallocate(error);
  return newError;
}

Result(void) wrapValue(void *value, ulong size) {
  return allocateCopy(size, NULL, value);
}

struct {
  Result(char) (*emptyError)();
  Result(char) (*error)(char*);
  Result(char) (*rethrown)(char*, char*);
  Result(void) (*wrap)(void*, ulong);
  void (*unwrap)(void*);
} Result = {
  createEmptyError,
  Error,
  rewrapError,
  wrapValue,
  deallocate
};

#endif
