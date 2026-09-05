#include "driver/keyboard.h"
#include "keyboardEventHandler.c"



void KeyboardDriver_init(KeyboardDriver* self, InterruptManager* im){

    Driver_Init(&self->driver); 
    self->driver.driverData = self;
    self->shift = 0;
    InterruptHandler_Init(&self->interruptHandler, 0x21, im);

    self->interruptHandler.HandleInterrupt = &KeyboardDriver_HandleInterrupt;

    Port8Bit_init(&self->dataport, 0x60);
    Port8Bit_init(&self->commandPort, 0x64);


    while (Port8Bit_Read(&self->commandPort) & 0x1)
    {
        Port8Bit_Read(&self->dataport);
    }

    Port8Bit_Write(&self->commandPort, 0xAE); // activate  keyboard communication
    Port8Bit_Write(&self->commandPort, 0x20); // get the current state

    uint8_t status = (Port8Bit_Read(&self->dataport) | 1) & ~0x10;

    Port8Bit_Write(&self->commandPort, 0x60);
    
    Port8Bit_Write(&self->dataport, status);

    Port8Bit_Write(&self->dataport, 0xF4);

    


}



uint32_t KeyboardDriver_HandleInterrupt(
    void *self,
    uint32_t esp
)
{
    KeyboardDriver *keyboard = (KeyboardDriver *)self;

    uint8_t key = Port8Bit_Read(&keyboard->dataport);

    /* Left Shift pressed */
    if (key == 0x2A)
    {
        keyboard->shift = 1;
        return esp;
    }

    /* Right Shift pressed */
    if (key == 0x36)
    {
        keyboard->shift = 1;
        return esp;
    }

    /* Left Shift released */
    if (key == 0xAA)
    {
        keyboard->shift = 0;
        return esp;
    }

    /* Right Shift released */
    if (key == 0xB6)
    {
        keyboard->shift = 0;
        return esp;
    }

    /* Ignore key releases */
    if (key >= 0x80)
        return esp;

    KeyboardEventHandler_onKeyDown(keyboard, key);

    return esp;
}