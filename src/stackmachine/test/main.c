#include <stdio.h>
#include <stdlib.h>

#include "../types.c"

#include "allocation.c"

#define useTest(function) \
  if (stringsMatch(args[1], #function)) return function()

boolean stringsMatch(const char *a, const char *b) {
  while (*a > 0 && *b > 0) {
    if (*a != *b) {
      return false;
    }
    ++a;
    ++b;
  }
  return true;
}

int main(int argCount, const char **args) {
  /*if (argCount < 2) {
    printf("Expected a test name.\n\n");
    printf("Usage: test <test-name>\n\n");
    return 1;
  }*/

  /* Managed by test.sh */ return 1;

  /*printf("Test %s does not exist.\n", args[1]);
  return 1;*/
}
