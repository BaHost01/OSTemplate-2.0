#ifndef IOMGR_H
#define IOMGR_H

#include "../common/win_types.h"

/* 
 * Standard NT I/O Major Function Codes
 */
#define IRP_MJ_CREATE                   0x00
#define IRP_MJ_CLOSE                    0x02
#define IRP_MJ_READ                     0x03
#define IRP_MJ_WRITE                    0x04
#define IRP_MJ_DEVICE_CONTROL           0x0e

/*
 * I/O Stack Location - Describes a single layer in the driver stack
 */
typedef struct _IO_STACK_LOCATION {
    uint8_t MajorFunction;
    uint8_t MinorFunction;
    uint8_t Flags;
    uint8_t Control;
    
    union {
        struct {
            uint32_t Length;
            uint32_t Key;
            uint64_t ByteOffset;
        } Read;
        struct {
            uint32_t Length;
            uint32_t Key;
            uint64_t ByteOffset;
        } Write;
    } Parameters;

    struct _DEVICE_OBJECT *DeviceObject;
} IO_STACK_LOCATION, *PIO_STACK_LOCATION;

/*
 * IRP (I/O Request Packet) - The primary vehicle for I/O requests
 */
typedef struct _IRP {
    CSHORT Type;
    uint16_t Size;
    PVOID AssociatedIrp;
    uint32_t Flags;
    uint32_t IoStatus; // Simplified Status
    KPROCESSOR_MODE RequestorMode;
    PVOID UserBuffer;
    
    struct {
        struct {
            PVOID SystemBuffer;
        } Overlay;
    } AssociatedIrpData;

    IO_STACK_LOCATION CurrentStackLocation; // Simplified to 1 layer
} IRP, *PIRP;

typedef struct _DEVICE_OBJECT {
    struct _DRIVER_OBJECT *DriverObject;
    struct _DEVICE_OBJECT *NextDevice;
    void* DeviceExtension;
    uint32_t DeviceType;
    uint32_t Flags;
} DEVICE_OBJECT, *PDEVICE_OBJECT;

typedef struct _DRIVER_OBJECT {
    CSHORT Type;
    CSHORT Size;
    PDEVICE_OBJECT DeviceObject;
    uint32_t Flags;
    PVOID DriverStart;
    uint32_t DriverSize;
    uint32_t (*MajorFunction[28])(PDEVICE_OBJECT DeviceObject, PIRP Irp);
} DRIVER_OBJECT, *PDRIVER_OBJECT;

void IoInit();
PIRP IoAllocateIrp(uint8_t StackSize);
uint32_t IoCallDriver(PDEVICE_OBJECT DeviceObject, PIRP Irp);
void IoCompleteRequest(PIRP Irp, uint8_t PriorityBoost);

#endif
