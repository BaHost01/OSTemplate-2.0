#include "iomgr.h"
#include "../common/vga.h"
#include "../common/pci.h"
#include "../common/win_types.h"

/*
 * WIFI_ADAPTER - Device extension for the Wifi driver.
 */
typedef struct _WIFI_ADAPTER {
    uint8_t mac_address[6];
    uint32_t channel;
    uint8_t connected;
} WIFI_ADAPTER, *PWIFI_ADAPTER;

/*
 * WifiDriverEntry - Initializes the Wifi driver object and creates its device.
 */
void WifiDriverEntry(PDRIVER_OBJECT DriverObject) {
    PDEVICE_OBJECT DeviceObject;
    
    /* Call the updated NT-style IoCreateDevice */
    IoCreateDevice(
        DriverObject, 
        sizeof(WIFI_ADAPTER), 
        NULL,           /* DeviceName - Optional */
        0x00000011,     /* DeviceType - FILE_DEVICE_NETWORK */
        0,              /* DeviceCharacteristics */
        0,              /* Exclusive */
        &DeviceObject
    );
    
    if (DeviceObject) {
        PWIFI_ADAPTER adapter = (PWIFI_ADAPTER)DeviceObject->DeviceExtension;
        adapter->connected = 0;
        vga_puts("[Wifi] Windows-Compatible Wifi Driver Loaded\n");
    }
}

/*
 * WifiScan - Simulates an 802.11 network scan.
 */
void WifiScan() {
    vga_puts("[Wifi] Scanning for networks (802.11 b/g/n)...\n");
    vga_puts("  Found: MyHome_WiFi (WPA2)\n");
    vga_puts("  Found: Coffee_Shop (Open)\n");
}
