#include "interrupts.h"
#include "../port/port.h"

extern void printf(char* str);
extern void printHex(uint8_t key);

GateDescriptor interruptDescriptorTable[256];
InterruptManager* ActiveInterruptManager = 0;

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1



uint32_t InterruptHandler_HandleInterrupt(
    void *self,
    uint32_t esp
){
    return esp;
}

void InterruptHandler_Init(InterruptHandler *self, uint8_t interruptNumber, InterruptManager *im){
    self->interruptNumber = interruptNumber;
    self->interruptManager = im;
    self->HandleInterrupt = InterruptHandler_HandleInterrupt;
    im->handlers[interruptNumber] = self; 
}

void InterruptHandler_Deactivate(InterruptHandler *self, uint8_t interruptNumber, InterruptManager *im){
    if (im->handlers[interruptNumber] == self){
        im->handlers[interruptNumber] = 0;
    }
}




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


void InterruptManager_Initialize(
    InterruptManager* self,
    GlobalDescriptorTable* gdt
){
    
    self->gdt = gdt;
    uint16_t codeSegment = GlobalDescriptorTable_CodeSegmentSelector(gdt);
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i =0; i < 256; i++){
        self->handlers[i] = 0;
        InterruptManager_SetInterruptDescriptorTableEntry(i, codeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    InterruptManager_SetInterruptDescriptorTableEntry(0x20, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    InterruptManager_SetInterruptDescriptorTableEntry(0x21, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    InterruptManager_SetInterruptDescriptorTableEntry(0x2C, codeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);

    Port8Bit_init(&self->picMasterCommand, PIC1_COMMAND);
    Port8Bit_init(&self->picMasterData,    PIC1_DATA);
    Port8Bit_init(&self->picSlaveCommand,  PIC2_COMMAND);
    Port8Bit_init(&self->picSlaveData,     PIC2_DATA);

    /* remap the PIC so IRQs land at interrupts 0x20-0x2F instead of
       colliding with the CPU's reserved exceptions 0x00-0x1F */
    Port8Bit_Write(&self->picMasterCommand, 0x11);
    Port8Bit_Write(&self->picSlaveCommand,  0x11);

    Port8Bit_Write(&self->picMasterData, 0x20); /* master offset = 0x20 */
    Port8Bit_Write(&self->picSlaveData,  0x28); /* slave offset  = 0x28 */

    Port8Bit_Write(&self->picMasterData, 0x04);
    Port8Bit_Write(&self->picSlaveData,  0x02);

    Port8Bit_Write(&self->picMasterData, 0x01);
    Port8Bit_Write(&self->picSlaveData,  0x01);

    Port8Bit_Write(&self->picMasterData, 0x00);
    Port8Bit_Write(&self->picSlaveData,  0x00);

    idt_pointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;

    // lidt is Load Interrupt Descriptor Table
    asm volatile("lidt %0": : "m" (idt));

}

void InterruptManager_Destroy(InterruptManager* self)
{
    if(ActiveInterruptManager == self){
        ActiveInterruptManager = 0;
        asm volatile("cli");
    }
    
    
}

void InterruptManager_Activate(InterruptManager* self){

    if(ActiveInterruptManager != 0){
        InterruptManager_Destroy(ActiveInterruptManager);
    }
    
    ActiveInterruptManager = self;
    asm("sti");
}

extern uint32_t DoHandleInterrupt(InterruptManager* self,uint8_t interruptNumber, uint32_t esp){

    if (self->handlers[interruptNumber] != 0)
    {
        InterruptHandler* handler =
            self->handlers[interruptNumber];

        esp = handler->HandleInterrupt(handler, esp);
    }

    else if(interruptNumber != 0x20){
        char* foo = "UNHANDLED INTERRUPT";
        printf(foo);
        printHex(interruptNumber);
    }
    

    // In x86 protected mode, the PIC hardware IRQs are commonly remapped to:

    // Interrupt	Hex	    IRQ
    // IRQ0     	0x20	Timer
    // IRQ1	        0x21	Keyboard
    // IRQ2	        0x22	Cascade
    // IRQ3	        0x23	COM2
    // ...	...	...
    // IRQ14	    0x2E	Primary ATA
    // IRQ15	    0x2F	Secondary ATA

    uint8_t hardware = 0x20 <= interruptNumber && interruptNumber < 0x30;

    if(hardware){
        Port8Bit_Write(&self->picMasterCommand, 0x20);
        
        if(0x28 <= interruptNumber){
            Port8Bit_Write(&self->picSlaveCommand, 0x20);
        }

    }

    return esp;
}


extern uint32_t InterruptManager_HandleInterrupt(uint8_t interruptNumber, uint32_t esp ){

    if(ActiveInterruptManager != 0){
        return DoHandleInterrupt(ActiveInterruptManager, interruptNumber, esp);
    }
    return esp;
}


