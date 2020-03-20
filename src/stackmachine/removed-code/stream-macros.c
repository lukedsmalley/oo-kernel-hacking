
#define StreamItem(type) \
  struct { \
    boolean done; \
    type value; \
  }

#define Stream(type) \
  struct { \
    byte valueSize; \
    void *handle; \
    struct { \
      boolean done; \
      type value; \
    } (*next)(type *value, void *handle); \
  }

#define doneStreamItem() \
  { \
    .done = true, \
    .value = 0 \
  }

#define doneItemForStreamOf(type) \
  (struct StreamItem) { \
    .done = true, \
    .value = 0 \
  }

#define streamItem(value) \
  { \
    .done = false, \
    .value = value \
  }

#define itemForStreamOf(type, value) \
  (struct { \
    boolean done; \
    type value; \
  }) { \
    .done = false, \
    .value = value \
  }

#define readStream(varRef, stream) \
  { \
    *varRef = 0 \
    ulong value = 0; \
    stream.next(&value, stream.handle); \
    for (byte i = 0; i < stream.valueSize; i++) { \
      *varRef <<= 8; \
      *varRef += value & 0xff; \
    } \
  }

typedef struct {
  boolean done;
  byte value;
} ByteStreamItem;

typedef struct {
  void *handle;
  ByteStreamItem (*next)(void *handle);
} ByteStream;

#define readStreamItemX()
