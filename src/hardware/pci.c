#include "hardwarecommunication/pci.h"

extern void printf(char* str);
extern void printHex(uint8_t key);

void PCI_Init(PeripheralComponentInterconnectDeviceController* self){
    Port32Bit_init(&self->dataPort, 0xCFC); 
    Port32Bit_init(&self->commandPort, 0xCF8);
}

uint32_t PCI_Read(PeripheralComponentInterconnectDeviceController* self, uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset){
    
    uint32_t id =
    0x1 << 31
    | ((bus & 0xFF) << 16)
    | ((device & 0x1F) << 11)
    | ((function & 0x07) << 8)
    | ((registeroffset & 0xFC))
    | 0x80000000; // Set the enable bit

    Port32Bit_Write(&self->commandPort, id);
    uint32_t result = Port32Bit_Read(&self->dataPort);

    return result >> (8 * (registeroffset % 4)); // Shift the result to get the correct byte
}

void PCI_Write(PeripheralComponentInterconnectDeviceController* self, uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value){
    uint32_t id =
    0x1 << 31
    | ((bus & 0xFF) << 16)
    | ((device & 0x1F) << 11)
    | ((function & 0x07) << 8)
    | ((registeroffset & 0xFC))
    | 0x80000000; // Set the enable bit

    Port32Bit_Write(&self->commandPort, id);
    Port32Bit_Write(&self->dataPort, value);
}

bool PCI_DeviceHasFunctions(PeripheralComponentInterconnectDeviceController* self, uint16_t bus, uint16_t device){
    uint32_t headerType = PCI_Read(self, bus, device, 0, 0x0E);
    return (headerType & 0x80) != 0;
}

void PCI_DeviceDescriptor_Init(PeripheralComponentInterconnectDeviceDescriptor* self, uint16_t bus, uint16_t device, uint16_t function){
    self->bus = bus;
    self->device = device;
    self->function = function;
}

PeripheralComponentInterconnectDeviceDescriptor PCI_GetDeviceDescriptor(PeripheralComponentInterconnectDeviceController* self, uint16_t bus, uint16_t device, uint16_t function){
    PeripheralComponentInterconnectDeviceDescriptor descriptor;
    PCI_DeviceDescriptor_Init(&descriptor, bus, device, function);

    uint32_t vendorID = PCI_Read(self, bus, device, function, 0x00);
    descriptor.vendor_id = vendorID;

    uint32_t deviceID = PCI_Read(self, bus, device, function, 0x02);
    descriptor.device_id = deviceID;

    descriptor.class_id = PCI_Read(self, bus, device, function, 0x0b);
    descriptor.subclass_id = PCI_Read(self, bus, device, function, 0x0a);
    descriptor.interface_id = PCI_Read(self, bus, device, function, 0x09);

    descriptor.revision = PCI_Read(self, bus, device, function, 0x08);
    descriptor.interrupt = PCI_Read(self, bus, device, function, 0x3c);

    return descriptor;
}

void PCI_SelectDrivers(PeripheralComponentInterconnectDeviceController* self, DriverManager* driverManager){
    for (uint16_t bus = 0; bus < 8; bus++){
        for (uint16_t device = 0; device < 32; device++){
            uint16_t functionCount = PCI_DeviceHasFunctions(self, bus, device) ? 8 : 1;
            for (uint16_t function = 0; function < functionCount; function++){
                
                PeripheralComponentInterconnectDeviceDescriptor descriptor = PCI_GetDeviceDescriptor(self, bus, device, function);

                // Here you would typically check the classID and subclassID to determine the type of device
                // and then select the appropriate driver from the driverManager.

                if (descriptor.vendor_id == 0x0000 || descriptor.device_id == 0xFFFF) {
                    // No device present
                    break;
                }

                printf("PCI BUS: ");
                printHex(descriptor.bus & 0xFF);
                printf(" DEVICE: ");
                printHex(descriptor.device & 0x1F);
                printf(" FUNCTION: ");
                printHex(descriptor.function & 0x07);

                printf(" VENDOR ID: ");
                printHex((descriptor.vendor_id & 0xFF00) >> 8);
                printHex(descriptor.vendor_id & 0xFF);

                printf(" DEVICE ID: ");
                printHex((descriptor.device_id & 0xFF00) >> 8);
                printHex(descriptor.device_id & 0xFF);
                printf("\n");

            }
        }
    }
}


