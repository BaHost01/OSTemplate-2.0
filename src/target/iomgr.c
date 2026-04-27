#include "iomgr.h"
#include "../common/kheap.h"
#include "../common/string.h"
#include "../common/vga.h"

void IoInit() {
    vga_puts("[IoMgr] Windows-Compatible I/O Manager Initialized\n");
}

uint32_t IoCreateDevice(PDRIVER_OBJECT DriverObject, uint32_t DeviceExtensionSize, PDEVICE_OBJECT *DeviceObject) {
    PDEVICE_OBJECT Device = (PDEVICE_OBJECT)kmalloc(sizeof(DEVICE_OBJECT));
    if (!Device) return 0xC000009A; // STATUS_INSUFFICIENT_RESOURCES

    memset(Device, 0, sizeof(DEVICE_OBJECT));
    Device->DriverObject = DriverObject;

    if (DeviceExtensionSize > 0) {
        Device->DeviceExtension = kmalloc(DeviceExtensionSize);
        memset(Device->DeviceExtension, 0, DeviceExtensionSize);
    }

    // Link into driver's device list
    Device->NextDevice = DriverObject->DeviceObject;
    DriverObject->DeviceObject = Device;

    *DeviceObject = Device;
    return 0; // STATUS_SUCCESS
}

void IoDeleteDevice(PDEVICE_OBJECT DeviceObject) {
    // In a real kernel, we would unlink and free the extension
    vga_puts("[IoMgr] Deleting Device Object\n");
    (void)DeviceObject;
}
