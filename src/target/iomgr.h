#ifndef IOMGR_H
#define IOMGR_H

#include "../common/win_types.h"

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
    // Standard dispatch routines
    void* MajorFunction[28];
} DRIVER_OBJECT, *PDRIVER_OBJECT;

void IoInit();
uint32_t IoCreateDevice(PDRIVER_OBJECT DriverObject, uint32_t DeviceExtensionSize, PDEVICE_OBJECT *DeviceObject);
void IoDeleteDevice(PDEVICE_OBJECT DeviceObject);

#endif
