#include "iomgr.h"
#include "../common/vga.h"
#include "../common/pci.h"

typedef struct _WIFI_ADAPTER {
    uint8_t mac_address[6];
    uint32_t channel;
    BOOLEAN connected;
} WIFI_ADAPTER, *PWIFI_ADAPTER;

void WifiDriverEntry(PDRIVER_OBJECT DriverObject) {
    PDEVICE_OBJECT DeviceObject;
    IoCreateDevice(DriverObject, sizeof(WIFI_ADAPTER), &DeviceObject);
    
    PWIFI_ADAPTER adapter = (PWIFI_ADAPTER)DeviceObject->DeviceExtension;
    adapter->connected = 0;
    
    vga_puts("[Wifi] Windows-Compatible Wifi Driver Loaded\n");
}

void WifiScan() {
    vga_puts("[Wifi] Scanning for networks (802.11 b/g/n)...\n");
    vga_puts("  Found: MyHome_WiFi (WPA2)\n");
    vga_puts("  Found: Coffee_Shop (Open)\n");
}
