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

BaseAddressRegister PCI_GetBaseAddressRegister(PeripheralComponentInterconnectDeviceController* self, uint16_t bus, uint16_t device, uint16_t function, uint16_t bar){
    BaseAddressRegister result;
    uint32_t headerType = PCI_Read(self, bus, device, function, 0x0E) & 0x7F;

    int maxBARs = (headerType == 0) ? 6 : 2; // Type 0 has 6 BARs, Type 1 has 2 BARs
    if (bar >= maxBARs) {
        return result; // Invalid BAR index
    }

    uint32_t barValue = PCI_Read(self, bus, device, function, 0x10 + bar * 4);
    result.type = (barValue & 0x1) ? InputOutput : MemoryMapping; // examin the last bit to determine the type
    uint32_t temp = 0;

    if (result.type == MemoryMapping) {
        temp = barValue & ~0xF; // Clear the last 4 bits for memory-mapped BAR

        switch ((barValue >> 1) & 0x3) {
            case 0: // 32-bit address
                result.size = 0xFFFFFFFF; // Placeholder for size, actual size determination requires writing and reading back
                break;
            case 2: // 64-bit address
                result.size = 0xFFFFFFFFFFFFFFFF; // Placeholder for size, actual size determination requires writing and reading back
                break;
            default:
                result.size = 0; // Invalid or unsupported type
                break;
        }

    } else {
        temp = barValue & ~0x3; // Clear the last 2 bits for I/O BAR
        result.prefetchable = 0;
    }
    result.address = (uint8_t*)temp;



    return result;
}

Driver* DriverManager_getDriver(PeripheralComponentInterconnectDeviceDescriptor* descriptor, DriverManager* driverManager, InterruptManager* interruptManager){
    // Here you would typically check the classID and subclassID to determine the type of device
    // and then return the appropriate driver. For example:

    switch(descriptor->vendor_id) {
        case 0x8086: // Intel
            // Check device_id for specific Intel devices and return the appropriate driver
            break;
        case 0x10EC: // Realtek
            // Check device_id for specific Realtek devices and return the appropriate driver
            break;
        // Add more vendor IDs and their corresponding drivers as needed

        case 0x1022: // AMD
            // Check device_id for specific AMD devices and return the appropriate driver

            switch(descriptor->device_id) {
                case 0x2000: // Example AMD device ID
                    // Return the appropriate driver for this AMD device
                    break;
                // Add more AMD device IDs and their corresponding drivers as needed
                default:
                    // Handle unknown AMD devices or return a generic driver if applicable
                    break;
            }

            break;

    }

    switch (descriptor->class_id) {
        case 0x03: // Display controller
            // Check subclass and interface for specific display types
            // Return the appropriate driver for the display controller
            break;
        case 0x02: // Network controller
            // Check subclass and interface for specific network types
            // Return the appropriate driver for the network controller
            break;
        case 0x01: // Mass storage controller
            // Check subclass and interface for specific storage types
            // Return the appropriate driver for the mass storage controller
            break;
    }

    // Add more conditions for other device types as needed

    return 0; // No suitable driver found
}



void PCI_SelectDrivers(PeripheralComponentInterconnectDeviceController* self, DriverManager* driverManager, InterruptManager* interruptManager){
    for (uint16_t bus = 0; bus < 8; bus++){
        for (uint16_t device = 0; device < 32; device++){
            uint16_t functionCount = PCI_DeviceHasFunctions(self, bus, device) ? 8 : 1;
            for (uint16_t function = 0; function < functionCount; function++){
                
                PeripheralComponentInterconnectDeviceDescriptor descriptor = PCI_GetDeviceDescriptor(self, bus, device, function);

                // Here you would typically check the classID and subclassID to determine the type of device
                // and then select the appropriate driver from the driverManager.

                if (descriptor.vendor_id == 0x0000 || descriptor.device_id == 0xFFFF) {
                    // No device present
                    continue;
                }

                for(uint16_t barNum = 0; barNum < 6; barNum++){
                    BaseAddressRegister bar = PCI_GetBaseAddressRegister(self, bus, device, function, barNum);
                    if (bar.address == 0 && bar.size == 0){
                        continue;
                    }
                    if(bar.address && (bar.type == InputOutput)){
                        descriptor.portBase = (uint32_t)bar.address;
                    }
                    else if(bar.address && (bar.type == MemoryMapping)){
                        // Handle memory-mapped BARs if needed
                    }

                    Driver* driver = DriverManager_getDriver(&descriptor, driverManager, interruptManager);
                    if (driver != 0){
                        DriverManager_addDriver(driverManager, driver);
                    }

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


