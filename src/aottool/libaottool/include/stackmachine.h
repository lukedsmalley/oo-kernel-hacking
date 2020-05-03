#include <stdlib.h>
#include <stdint.h>

typedef size_t id_t;

typedef struct {
  uint8_t *items;
  size_t count;
} uint8_vector_t;

typedef struct {
  class_vector_t classes;
  id_t entryClass;
  id_t entryMethod;
} assembly_t;

typedef struct {
  assembly_t *items;
  size_t count;
} assembly_vector_t;

typedef struct {

} call_result_t;

call_result_t runAssembly(assembly_t assembly);
