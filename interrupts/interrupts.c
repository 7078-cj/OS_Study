#include "interrupts.h"
#include "../port/port.h"

extern void printf(char* str);

GateDescriptor interruptDescriptorTable[256];

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

void InterruptManager_SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t gdt_codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType
){
    
    
    const uint8_t IDT_DESC_PRESENT = 0x80;


    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = gdt_codeSegmentSelectorOffset;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | (DescriptorPrivilegeLevel << 5) ;
    interruptDescriptorTable[interruptNumber].reserved = 0 ;

}


void InterruptManager_Initialize(GlobalDescriptorTable* gdt){

    uint16_t codeSegment = GlobalDescriptorTable_CodeSegmentSelector(gdt);
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i =0; i < 256; i++){
        InterruptManager_SetInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    InterruptManager_SetInterruptDescriptorTableEntry(0x20, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    InterruptManager_SetInterruptDescriptorTableEntry(0x21, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);

    Port8Bit_init(&picMasterCommand, PIC1_COMMAND);
    Port8Bit_init(&picMasterData,    PIC1_DATA);
    Port8Bit_init(&picSlaveCommand,  PIC2_COMMAND);
    Port8Bit_init(&picSlaveData,     PIC2_DATA);

    /* remap the PIC so IRQs land at interrupts 0x20-0x2F instead of
       colliding with the CPU's reserved exceptions 0x00-0x1F */
    Port8Bit_Write(&picMasterCommand, 0x11);
    Port8Bit_Write(&picSlaveCommand,  0x11);

    Port8Bit_Write(&picMasterData, 0x20); /* master offset = 0x20 */
    Port8Bit_Write(&picSlaveData,  0x28); /* slave offset  = 0x28 */

    Port8Bit_Write(&picMasterData, 0x04);
    Port8Bit_Write(&picSlaveData,  0x02);

    Port8Bit_Write(&picMasterData, 0x01);
    Port8Bit_Write(&picSlaveData,  0x01);

    Port8Bit_Write(&picMasterData, 0x00);
    Port8Bit_Write(&picSlaveData,  0x00);

    idt_pointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;

    // lidt is Load Interrupt Descriptor Table
    asm volatile("lidt %0": : "m" (idt));

}

void InterruptManager_Destroy(void)
{
    asm volatile("cli");
}

void Activate(){
    asm("sti");
}

extern uint32_t InterruptManager_HandleInterrupt(uint8_t interruptNumber, uint32_t esp ){

    printf("interrupt");

    return esp;
}


