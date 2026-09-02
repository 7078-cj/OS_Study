#include "mouse.h"



void MouseDriver_init(MouseDriver* self, InterruptManager* im){
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

    mouse->buffer[mouse->offset] = Port8Bit_Read(&mouse->dataport);
    mouse->offset = (mouse->offset + 1) % 3;

    static int8_t x = 40;
    static int8_t y = 12;
    static uint8_t cursorHighlighted = 0; // is an extra click-invert currently on the cursor cell?

    if (mouse->offset == 0)
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;

        // erase old cursor: undo the cursor-invert, and undo the click-invert if it's still there
        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4)
                                | ((VideoMemory[80 * y + x] & 0x0F00) << 4)
                                | (VideoMemory[80 * y + x] & 0x00FF);

        if (cursorHighlighted)
        {
            VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4)
                                    | ((VideoMemory[80 * y + x] & 0x0F00) << 4)
                                    | (VideoMemory[80 * y + x] & 0x00FF);
            cursorHighlighted = 0;
        }

        x += mouse->buffer[1];
        if (x < 0) x = 0;
        if (x >= 80) x = 79;

        y -= mouse->buffer[2];
        if (y < 0) y = 0;
        if (y >= 25) y = 24;

        // draw new cursor
        VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4)
                                | ((VideoMemory[80 * y + x] & 0x0F00) << 4)
                                | (VideoMemory[80 * y + x] & 0x00FF);

        // re-apply click highlight every packet while any button is held (not just on the edge)
        if (mouse->buffer[0] & 0x07)
        {
            VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4)
                                    | ((VideoMemory[80 * y + x] & 0x0F00) << 4)
                                    | (VideoMemory[80 * y + x] & 0x00FF);
            cursorHighlighted = 1;
        }

        mouse->buttons = mouse->buffer[0];
    }

    return esp;
}