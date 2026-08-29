#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../types.h"
#include "../port/port.h"
#include "../gdt/gdt.h"


typedef struct GateDescriptor
{
    uint16_t handlerAddressLowBits;
    uint16_t gdt_codeSegmentSelector;
    uint8_t  reserved;
    uint8_t  access;
    uint16_t handlerAddressHighBits; 

} __attribute__((packed)) GateDescriptor;


extern GateDescriptor interruptDescriptorTable[256];

typedef struct idt_pointer
    {
        uint16_t size;
        uint32_t base;
    } __attribute__((packed)) idt_pointer;

void InterruptManager_SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t gdt_codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType
);

void InterruptManager_Initialize(GlobalDescriptorTable* gdt);
void InterruptManager_Destroy(void);

void Activate();

uint32_t InterruptManager_HandleInterrupt(uint8_t interruptNumber, uint32_t esp);


static Port8Bit picMasterCommand;
static Port8Bit picMasterData;
static Port8Bit picSlaveCommand;
static Port8Bit picSlaveData;


extern void IgnoreInterruptRequest();

extern void HandleInterruptRequest0x00();
extern void HandleInterruptRequest0x01();

#endif