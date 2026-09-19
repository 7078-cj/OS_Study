#include "common/types.h"
#include "gdt.h"
#include "hardwarecommunication/port.h"
#include "hardwarecommunication/interrupts.h"
#include "driver/keyboard.h"
#include "driver/mouse.h"
#include "driver/driver.h"
#include "hardwarecommunication/pci.h"
#include "driver/vga.h"
#include "gui/desktop.h"
#include "gui/widget.h"
#include "gui/window.h"

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
void printHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0x0F];
    foo[1] = hex[key & 0x0f];
    printf(foo);
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

    DriverManager dm;
    DriverManager_init(&dm);

    KeyboardDriver kd;
    KeyboardDriver_init(&kd, &im);
    DriverManager_addDriver(&dm, &kd.driver);

    MouseDriver md;
    MouseDriver_init(&md, &im);
    DriverManager_addDriver(&dm, &md.driver);

    PeripheralComponentInterconnectDeviceController pciController;
    PCI_Init(&pciController);
    PCI_SelectDrivers(&pciController, &dm, &im);

    VideoGraphicsArray vga;
    VideoGraphicsArray_init(&vga);
    DriverManager_addDriver(&dm, &vga.driver);

    DriverManager_activate(&dm);


    VideoGraphicsArray_setMode(
        &vga,
        320,
        200,
        8
    );

    InterruptManager_Activate(&im);


    Desktop desktop;
    Desktop_Init(&desktop, &kd, &md, 320, 200, 0x00, 0x00, 0xA8);

    
    

    while(1){
        desktop.Draw(&desktop, &vga);
    }
}