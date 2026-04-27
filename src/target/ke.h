#ifndef KE_H
#define KE_H

#include "../common/win_types.h"

typedef struct _DISPATCHER_HEADER {
    uint8_t Type;
    uint8_t Absolute;
    uint8_t Size;
    uint8_t Inserted;
    int32_t SignalState;
    LIST_ENTRY WaitListHead;
} DISPATCHER_HEADER;

typedef struct _KEVENT {
    DISPATCHER_HEADER Header;
} KEVENT, *PKEVENT;

void KeInitializeEvent(PKEVENT Event, uint32_t Type, BOOLEAN State);
KIRQL KeGetCurrentIrql();
KIRQL KfRaiseIrql(KIRQL NewIrql);
void KfLowerIrql(KIRQL NewIrql);

#endif
