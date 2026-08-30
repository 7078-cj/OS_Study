#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../types.h"
#include "../port/port.h"
#include "../gdt/gdt.h"

typedef struct InterruptManager InterruptManager;
typedef struct InterruptHandler InterruptHandler;


typedef struct InterruptManager {
    GlobalDescriptorTable* gdt;
    Port8Bit picMasterCommand;
    Port8Bit picMasterData;
    Port8Bit picSlaveCommand;
    Port8Bit picSlaveData;
    InterruptHandler* handlers[256];

} InterruptManager;

typedef struct  InterruptHandler
{
    uint8_t interruptNumber;
    InterruptManager* interruptManager;

    uint32_t (*HandleInterrupt)(void* self, uint32_t esp);


} InterruptHandler;

void InterruptHandler_Init(InterruptHandler *self, uint8_t interruptNumber, InterruptManager *im);
void InterruptHandler_Deactivate(InterruptHandler *self, uint8_t interruptNumber, InterruptManager *im);
uint32_t InterruptHandler_HandleInterrupt(void* self, uint32_t esp);



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

void InterruptManager_Initialize(InterruptManager* self, GlobalDescriptorTable* gdt);
void InterruptManager_Destroy(InterruptManager* self);

void Activate(InterruptManager* self);

uint32_t InterruptManager_HandleInterrupt(uint8_t interruptNumber, uint32_t esp);

uint32_t DoHandleInterrupt(InterruptManager* self,uint8_t interruptNumber, uint32_t esp);

extern void IgnoreInterruptRequest();

extern void HandleInterruptRequest0x00();
extern void HandleInterruptRequest0x01();

#endif