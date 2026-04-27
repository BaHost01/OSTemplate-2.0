#include "ke.h"
#include "../common/stdio.h"

static KIRQL CurrentIrql = PASSIVE_LEVEL;

KIRQL KeGetCurrentIrql() {
    return CurrentIrql;
}

KIRQL KfRaiseIrql(KIRQL NewIrql) {
    KIRQL OldIrql = CurrentIrql;
    if (NewIrql > CurrentIrql) {
        CurrentIrql = NewIrql;
        // In real hardware, we'd update the CR8 register or APIC TPR here
    }
    return OldIrql;
}

void KfLowerIrql(KIRQL NewIrql) {
    if (NewIrql < CurrentIrql) {
        CurrentIrql = NewIrql;
    }
}

void KeInitializeEvent(PKEVENT Event, uint32_t Type, BOOLEAN State) {
    Event->Header.Type = Type;
    Event->Header.SignalState = State;
    Event->Header.WaitListHead.Flink = &Event->Header.WaitListHead;
    Event->Header.WaitListHead.Blink = &Event->Header.WaitListHead;
    kprintf("[Kernel] Event Initialized @ %p\n", (uint64_t)Event);
}
