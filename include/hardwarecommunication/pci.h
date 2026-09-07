#ifndef __PCI_H
#define __PCI_H

#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "driver/driver.h"

typedef struct PeripheralComponentInterconnectDeviceDescriptor
{
    uint32_t portBase;
    uint32_t interrupt;

    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;

    uint8_t revision;

} PeripheralComponentInterconnectDeviceDescriptor;

void PCI_DeviceDescriptor_Init(PeripheralComponentInterconnectDeviceDescriptor* self, uint16_t bus, uint16_t device, uint16_t function);

typedef struct PeripheralComponentInterconnectDeviceController
{
    Port32Bit dataPort;
    Port32Bit commandPort;
} PeripheralComponentInterconnectDeviceController;


void PCI_Init(PeripheralComponentInterconnectDeviceController* self);

uint32_t PCI_Read(PeripheralComponentInterconnectDeviceController* self, uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset);
void PCI_Write(PeripheralComponentInterconnectDeviceController* self, uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value);
bool PCI_DeviceHasFunctions(PeripheralComponentInterconnectDeviceController* self, uint16_t bus, uint16_t device);

void PCI_SelectDrivers(PeripheralComponentInterconnectDeviceController* self, DriverManager* driverManager);
PeripheralComponentInterconnectDeviceDescriptor PCI_GetDeviceDescriptor(PeripheralComponentInterconnectDeviceController* self, uint16_t bus, uint16_t device, uint16_t function);



#endif // __PCI_H