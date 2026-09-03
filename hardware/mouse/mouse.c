#include "mouse.h"
#include "mouseEventHandler.c"



void MouseDriver_init(MouseDriver* self, InterruptManager* im){

    Driver_Init(&self->driver); 
    self->driver.driverData = self;
    InterruptHandler_Init(&self->interruptHandler, 0x2C, im);

    self->interruptHandler.HandleInterrupt = &MouseDriver_HandleInterrupt;

    Port8Bit_init(&self->dataport, 0x60);
    Port8Bit_init(&self->commandPort, 0x64);

    self->offset = 0;
    self->buttons = 0;
    self->buffer[0] = 0;
    self->buffer[1] = 0;
    self->buffer[2] = 0;

    uint16_t* VideoMemory = (uint16_t*)0xb8000;

    VideoMemory[80 * 12 + 40] = ((VideoMemory[80 * 12 + 40] & 0xF000) >> 4)
                            | ((VideoMemory[80 * 12 + 40] & 0x0F00) << 4)
                            | (VideoMemory[80 * 12 + 40] & 0x00FF);

    Port8Bit_Write(&self->commandPort, 0xA8); // activate  keyboard communication
    Port8Bit_Write(&self->commandPort, 0x20); // get the current state

    uint8_t status = Port8Bit_Read(&self->dataport) | 2;

    Port8Bit_Write(&self->commandPort, 0x60);
    
    Port8Bit_Write(&self->dataport, status);

    Port8Bit_Write(&self->commandPort, 0xD4);
    Port8Bit_Write(&self->dataport, 0xF4);

    Port8Bit_Read(&self->dataport);

    


}

void printf(char* str);

uint32_t MouseDriver_HandleInterrupt(
    void *self,
    uint32_t esp
)
{
    MouseDriver *mouse = (MouseDriver *)self;
    uint8_t status = Port8Bit_Read(&mouse->commandPort);

    if (!(status & 0x20))
    {
        return esp;
    }

    MouseEventHandler_onKeyDown(mouse);

    return esp;
}