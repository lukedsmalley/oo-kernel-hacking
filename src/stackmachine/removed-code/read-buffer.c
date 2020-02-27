#include "types.c"
#include "nice-things.c"

long readLong(const byte *buffer, ulong bufferLength, ulong position,
    const char *outOfBoundsMessage, const char* tooShortMessage) {
  if (position > bufferLength) {
    printf(outOfBoundsMessage);
    niceExit(1);
  } else if (position + 8 > bufferLength) {
    printf(tooShortMessage);
    niceExit(1);
  }
  return buffer[position] + ((long)buffer[++position] << 8) +
    ((long)buffer[++position] << 16) + ((long)buffer[++position] << 24) +
    ((long)buffer[++position] << 32) + ((long)buffer[++position] << 40) +
    ((long)buffer[++position] << 48) + ((long)buffer[++position] << 56);
}
