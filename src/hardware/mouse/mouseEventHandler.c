#include "driver/mouse.h"

void printf(char* str);

void MouseCursorInvert(int8_t* x, int8_t* y)
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;

    int offset = (*y) * 80 + (*x);   // dereference — these are pointers, not coordinates

    VideoMemory[offset] =
        ((VideoMemory[offset] & 0xF000) >> 4) |
        ((VideoMemory[offset] & 0x0F00) << 4) |
        (VideoMemory[offset] & 0x00FF);
}


void updateLocation(MouseDriver* mouse, int8_t* x, int8_t* y)
{
    *x += mouse->buffer[1];
    if (*x < 0)   *x = 0;
    if (*x >= 80) *x = 79;

    *y -= mouse->buffer[2];
    if (*y < 0)   *y = 0;
    if (*y >= 25) *y = 24;
}


void onMouseMove(MouseDriver* mouse, int8_t* x, int8_t* y, uint8_t* cursorHighlighted)
{
    MouseCursorInvert(x, y);

    if (*cursorHighlighted)
    {
        MouseCursorInvert(x, y);
        *cursorHighlighted = 0;
    }

    updateLocation(mouse, x, y);


    MouseCursorInvert(x, y);


    if (mouse->buffer[0] & 0x07)
    {
        MouseCursorInvert(x, y);
        *cursorHighlighted = 1;
    }
}



void onMouseClick(uint8_t button, uint8_t previousButtons)
{
    if ((button & 0x01) && !(previousButtons & 0x01))
        printf("Left Click\n");

    if ((button & 0x02) && !(previousButtons & 0x02))
        printf("Right Click\n");

    if ((button & 0x04) && !(previousButtons & 0x04))
        printf("Middle Click\n");
}



void MouseReadPacket(MouseDriver* mouse)
{
    uint8_t key = Port8Bit_Read(&mouse->dataport);
    mouse->buffer[mouse->offset] = key;
    mouse->offset = (mouse->offset + 1) % 3;
}


void MouseEventHandler_onKeyDown(void* self)
{
    MouseDriver* mouse = (MouseDriver*)self;

    MouseReadPacket(mouse);

    if (mouse->offset != 0)
        return;

    static int8_t x = 40;
    static int8_t y = 12;
    static uint8_t cursorHighlighted = 0;

    onMouseMove(mouse, &x, &y, &cursorHighlighted);
    onMouseClick(mouse->buffer[0], mouse->buttons);

    mouse->buttons = mouse->buffer[0];
}