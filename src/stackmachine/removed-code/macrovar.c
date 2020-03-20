
boolean macroVarLocked = false;
void *macroVar = 0;

#define requestMacroVar(type) \
  while (macroVarLocked); \
  macroVarLocked = true; \
  macroVar = allocFromDefaultHeap(sizeof(type))

#define releaseMacroVar() \
  deallocFromDefaultHeap(macroVar); \
  macroVarLocked = false

typedef struct {
  StreamItem count;
} ReadListOfMacroVar;

#define readListOf(list, reader, stream, countSize) \
  requestMacroVar(ReadListOfMacroVar); \
  macroVar->count = readAndCombineStreamItems(stream, countSize); \
  for (ulong i = 0; i < macroVar; i++) { \
    addToList(reader() \
  }
