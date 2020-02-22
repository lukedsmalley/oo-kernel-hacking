#include <stdint.h>

typedef struct {
  uint8_t buffer[4096];
  int top;
} stack_t;

short readShort(const uint8_t *buffer, int position) {
  return buffer[position] + ((short)buffer[++position] << 8);
}

int readInt(const uint8_t *buffer, int position) {
  return buffer[position] + ((int)buffer[++position] << 8) +
    ((int)buffer[++position] << 16) + ((int)buffer[++position] << 24);
}

long readLong(const uint8_t *buffer, int position) {
  return buffer[position] + ((long)buffer[++position] << 8) +
    ((long)buffer[++position] << 16) + ((long)buffer[++position] << 24) +
    ((long)buffer[++position] << 32) + ((long)buffer[++position] << 40) +
    ((long)buffer[++position] << 48) + ((long)buffer[++position] << 56);
}
