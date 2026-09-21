#ifndef __DESKTOP_H            
#define __DESKTOP_H  

#include "gui/widget.h"

typedef struct Desktop
{
    /* data */
    MouseEventHandler mouseEventHandler;
    KeyboardEventHandler keyboardEventHandler; 

    CompositeWidget compositeWidget;
    KeyboardDriver *keyboard;
    MouseDriver *mouse;

    uint32_t MouseX;
    uint32_t MouseY;

    volatile bool needsRedraw;

    void (*Draw)(void* self, GraphicsContext *gc);
    void (*OnMouseDown)(void* self, int32_t x, int32_t y, uint8_t button);
    void (*OnMouseUp)(void* self, int32_t x, int32_t y, uint8_t button);
    void (*OnMouseMove)(void* self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);

} Desktop;

void Desktop_Init(
    Desktop *self, 
    KeyboardDriver *keyboard, 
    MouseDriver *mouse,     
    int32_t w,
    int32_t h,
    uint8_t r,
    uint8_t g,
    uint8_t b
);
void Desktop_draw(void* self, GraphicsContext *gc);

void Desktop_onMouseDown(void *self, int32_t x, int32_t y, uint8_t button);
void Desktop_onMouseUp(void *self, int32_t x, int32_t y, uint8_t button);
void Desktop_onMouseMove(void *self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);

void Desktop_MouseMoveEvent(void* self, int8_t x, int8_t y);
void Desktop_MouseUpEvent(void* self, uint8_t button);
void Desktop_MouseDownEvent(void* self, uint8_t button);

void Desktop_KeyDownEvent(void* self, char* key);
void Desktop_KeyUpEvent(void* self, char* key);

#endif