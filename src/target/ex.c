#include "../common/win_types.h"
#include "../common/kheap.h"
#include "../common/stdio.h"

// Types of Pool
typedef enum {
    NonPagedPool,
    PagedPool,
    NonPagedPoolMustSucceed,
    DontUseThisType
} POOL_TYPE;

PVOID ExAllocatePoolWithTag(POOL_TYPE PoolType, size_t NumberOfBytes, uint32_t Tag) {
    (void)PoolType; // Simplified to always use our heap
    
    PVOID ptr = kmalloc(NumberOfBytes);
    if (!ptr) {
        kprintf("[EX] Failed to allocate pool for tag: %x\n", (uint64_t)Tag);
        return NULL;
    }
    
    // In real Windows, the tag is stored before the pointer
    return ptr;
}

void ExFreePoolWithTag(PVOID P, uint32_t Tag) {
    (void)Tag;
    kfree(P);
}

void RtlInitUnicodeString(PUNICODE_STRING DestinationString, uint16_t* SourceString) {
    uint16_t length = 0;
    if (SourceString) {
        while (SourceString[length]) length++;
        DestinationString->Length = length * sizeof(uint16_t);
        DestinationString->MaximumLength = DestinationString->Length + sizeof(uint16_t);
    } else {
        DestinationString->Length = 0;
        DestinationString->MaximumLength = 0;
    }
    DestinationString->Buffer = SourceString;
}
