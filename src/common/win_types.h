#ifndef WIN_TYPES_H
#define WIN_TYPES_H

#include <stdint.h>

typedef int8_t CCHAR;
typedef int16_t CSHORT;
typedef int32_t LONG;
typedef uint32_t ULONG;
typedef uint64_t ULONG_PTR;
typedef uint8_t BOOLEAN;
typedef void* PVOID;

typedef CCHAR KPROCESSOR_MODE;

typedef struct _UNICODE_STRING {
    uint16_t Length;
    uint16_t MaximumLength;
    uint16_t* Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef enum _MODE {
    KernelMode,
    UserMode,
    MaximumMode
} MODE;

typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY;

// NT Status Codes
#define STATUS_SUCCESS          ((uint32_t)0x00000000L)
#define STATUS_UNSUCCESSFUL     ((uint32_t)0xC0000001L)
#define STATUS_INSUFFICIENT_RESOURCES ((uint32_t)0xC000009AL)

// IRQL levels
#define PASSIVE_LEVEL 0
#define APC_LEVEL     1
#define DISPATCH_LEVEL 2
#define DIRQL         3

typedef uint8_t KIRQL;

#endif
