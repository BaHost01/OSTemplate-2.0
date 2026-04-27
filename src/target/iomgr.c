#include "iomgr.h"
#include "ke.h"
#include "../common/kheap.h"
#include "../common/string.h"
#include "../common/stdio.h"

/*
 * IoInit - Initializes the NT-compatible I/O subsystem.
 */
void IoInit() {
    kprintf("[IoMgr] NT-Standard I/O Subsystem Online\n");
}

/*
 * IoAllocateIrp - Allocates an I/O Request Packet.
 */
PIRP IoAllocateIrp(uint8_t StackSize) {
    (void)StackSize;
    PIRP Irp = (PIRP)kmalloc(sizeof(IRP));
    if (Irp) {
        memset(Irp, 0, sizeof(IRP));
    }
    return Irp;
}

/*
 * IoCallDriver - Dispatches an IRP to the appropriate driver major function.
 */
uint32_t IoCallDriver(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    PDRIVER_OBJECT DriverObject = DeviceObject->DriverObject;
    uint8_t major = Irp->CurrentStackLocation.MajorFunction;

    if (major > 27 || !DriverObject->MajorFunction[major]) {
        return STATUS_UNSUCCESSFUL;
    }

    /* Standard NT call: major function receives device and the packet */
    return DriverObject->MajorFunction[major](DeviceObject, Irp);
}

/*
 * IoCompleteRequest - Finalizes an I/O operation.
 */
void IoCompleteRequest(PIRP Irp, uint8_t PriorityBoost) {
    (void)PriorityBoost;
    kprintf("[IoMgr] Request Completed Status: %x\n", (uint64_t)Irp->IoStatus);
    kfree(Irp);
}

/*
 * IoCreateDevice - Professional device object creation.
 */
uint32_t IoCreateDevice(
    PDRIVER_OBJECT DriverObject,
    uint32_t DeviceExtensionSize,
    PUNICODE_STRING DeviceName,
    uint32_t DeviceType,
    uint32_t DeviceCharacteristics,
    BOOLEAN Exclusive,
    PDEVICE_OBJECT *DeviceObject
) {
    (void)DeviceName; (void)DeviceCharacteristics; (void)Exclusive;

    PDEVICE_OBJECT Device = (PDEVICE_OBJECT)kmalloc(sizeof(DEVICE_OBJECT));
    if (!Device) return STATUS_INSUFFICIENT_RESOURCES;

    memset(Device, 0, sizeof(DEVICE_OBJECT));
    Device->DriverObject = DriverObject;
    Device->DeviceType = DeviceType;

    if (DeviceExtensionSize > 0) {
        Device->DeviceExtension = kmalloc(DeviceExtensionSize);
        if (!Device->DeviceExtension) {
            kfree(Device);
            return STATUS_INSUFFICIENT_RESOURCES;
        }
        memset(Device->DeviceExtension, 0, DeviceExtensionSize);
    }

    Device->NextDevice = DriverObject->DeviceObject;
    DriverObject->DeviceObject = Device;

    *DeviceObject = Device;
    return STATUS_SUCCESS;
}
