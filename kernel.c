#include "types.h"
#include "gdt/gdt.h"
#include "port/port.h"
#include "interrupts/interrupts.h"
#include "hardware/keyboard/keyboard.h"
#include "hardware/mouse/mouse.h"

uint16_t* VideoMemory = (uint16_t*)0xb8000;

uint8_t CursorX = 0;
uint8_t CursorY = 0;

void printf(char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        switch (str[i])
        {
            case '\n':
                CursorX = 0;
                CursorY++;
                break;

            default:
            {
                int offset = CursorY * 80 + CursorX;

                VideoMemory[offset] =
                    (VideoMemory[offset] & 0xFF00) | str[i];

                CursorX++;

                if (CursorX >= 80)
                {
                    CursorX = 0;
                    CursorY++;
                }

                break;
            }
        }
    }
}

typedef void (*constructor)(void);

extern constructor start_ctors;
extern constructor end_ctors;

void callConstructors(){
    for (constructor* i = &start_ctors; i != &end_ctors; i++) {
        (*i)();
    }
}
void kernelMain(void* multiboot_structure, unsigned int magic_number){
    printf("Hello World WOW \n");
    printf("Wrahhhhh \n");

    GlobalDescriptorTable gdt;
    InterruptManager im;

    // 1. Build the actual GDT
    GlobalDescriptorTable_Init(&gdt);

    //2. initialize the Interrupt table
    InterruptManager_Initialize(&im, &gdt);

    KeyboardDriver kd;
    KeyboardDriver_init(&kd, &im);

    MouseDriver md;
    MouseDriver_init(&md, &im);

    InterruptManager_Activate(&im);

    while(1){
    }
}