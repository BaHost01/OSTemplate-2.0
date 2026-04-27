#ifndef WIN_TYPES_H
#define WIN_TYPES_H

#include <stdint.h>

typedef int8_t CCHAR;
typedef int16_t CSHORT;
typedef int32_t LONG;
typedef uint32_t ULONG;
typedef uint8_t BOOLEAN;
typedef void* PVOID;

typedef CCHAR KPROCESSOR_MODE;

typedef enum _MODE {
    KernelMode,
    UserMode,
    MaximumMode
} MODE;

typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;

// IRQL levels (NT style)
#define PASSIVE_LEVEL 0
#define APC_LEVEL     1
#define DISPATCH_LEVEL 2
#define DIRQL         3

typedef uint8_t KIRQL;

#endif
