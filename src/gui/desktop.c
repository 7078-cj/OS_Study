#include "gui/desktop.h"
#include "gui/window.h"



void Desktop_Init(Desktop *self, KeyboardDriver *keyboard, MouseDriver *mouse, int32_t w, int32_t h, uint8_t r, uint8_t g, uint8_t b)
{
    self->keyboard = keyboard;
    self->mouse = mouse;
    CompositeWidget_init(&self->compositeWidget, keyboard, mouse, 0, 0, 0, w, h, r, g, b);

    self->MouseX = w / 2;
    self->MouseY = h / 2;
    self->needsRedraw = true;

    self->Draw          = Desktop_draw;
    self->OnMouseDown    = Desktop_onMouseDown;
    self->OnMouseUp      = Desktop_onMouseUp;
    self->OnMouseMove    = Desktop_onMouseMove;

    self->OnKeyDown = &Desktop_KeyDown;
    self->OnKeyUp   = &Desktop_KeyUp;

    /* implement the mouse-driver-facing interface */
    self->mouseEventHandler.OnMouseDown = &Desktop_MouseDownEvent;
    self->mouseEventHandler.OnMouseUp   = &Desktop_MouseUpEvent;
    self->mouseEventHandler.OnMouseMove = &Desktop_MouseMoveEvent;
    MouseDriver_setHandler(mouse, &self->mouseEventHandler);

    self->keyboardEventHandler.OnKeyDown = &Desktop_KeyDownEvent;   
    self->keyboardEventHandler.OnKeyUp   = &Desktop_KeyUpEvent;   
    KeyboardDriver_setHandler(keyboard, &self->keyboardEventHandler);
}

void Desktop_draw(void* self, GraphicsContext *gc)
{
    Desktop *desktop = (Desktop *)self;

    desktop->compositeWidget.Draw(
        &desktop->compositeWidget,
        gc
    );

    for (int i = 0; i < 4; i++)
    {
        PutPixel(
            gc,
            desktop->MouseX + i,
            desktop->MouseY,
            0xFF, 0xFF, 0xFF
        );

        PutPixel(
            gc,
            desktop->MouseX,
            desktop->MouseY + i,
            0xFF, 0xFF, 0xFF
        );
    }
}

void Desktop_onMouseDown(void *self, int32_t x, int32_t y, uint8_t button){
    Desktop *desktop =
        (Desktop *)self;

    desktop->compositeWidget.OnMouseDown(&desktop->compositeWidget,x, y, button);
}
void Desktop_onMouseUp(void *self, int32_t x, int32_t y, uint8_t button){
    Desktop *desktop =
        (Desktop *)self;

    desktop->compositeWidget.OnMouseUp(&desktop->compositeWidget,x, y, button);
}

void Desktop_MouseDownEvent(void* self, uint8_t button)
{
    Desktop* desktop = (Desktop*)self;
    desktop->OnMouseDown(desktop, desktop->MouseX, desktop->MouseY, button);
    desktop->needsRedraw = true;
}

void Desktop_MouseUpEvent(void* self, uint8_t button)
{
    Desktop* desktop = (Desktop*)self;
    desktop->OnMouseUp(desktop, desktop->MouseX, desktop->MouseY, button);
    desktop->needsRedraw = true;
}

void Desktop_MouseMoveEvent(void* self, int8_t x, int8_t y)
{
    Desktop* desktop = (Desktop*)self;

    int32_t newMouseX = desktop->MouseX + x;
    if (newMouseX < 0) newMouseX = 0;
    if (newMouseX >= desktop->compositeWidget.w) newMouseX = desktop->compositeWidget.w - 1;

    int32_t newMouseY = desktop->MouseY - y;   // PS/2: up is positive — flip for screen rows
    if (newMouseY < 0) newMouseY = 0;
    if (newMouseY >= desktop->compositeWidget.h) newMouseY = desktop->compositeWidget.h - 1;

    desktop->OnMouseMove(desktop, desktop->MouseX, desktop->MouseY, newMouseX, newMouseY);

    desktop->MouseX = newMouseX;
    desktop->MouseY = newMouseY;
    desktop->needsRedraw = true;
}

void Desktop_onMouseMove(void *self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y)
{
    Desktop *desktop = (Desktop *)self;
    desktop->compositeWidget.OnMouseMove(&desktop->compositeWidget, old_x, old_y, new_x, new_y);
    
}

void Desktop_KeyDown(void* self, char* key)
{
    Desktop* desktop = (Desktop*)self;
    desktop->compositeWidget.OnKeyDown(&desktop->compositeWidget, key);
    desktop->needsRedraw = true;
}

void Desktop_KeyUp(void* self, char* key)
{
    Desktop* desktop = (Desktop*)self;
    desktop->compositeWidget.OnKeyUp(&desktop->compositeWidget, key);
    desktop->needsRedraw = true;
}

void Desktop_KeyDownEvent(void* self, char* key)
{
    Desktop* desktop =
    (Desktop*)((char*)self - offsetof(Desktop, keyboardEventHandler));
    desktop->OnKeyDown(desktop, key);
}

void Desktop_KeyUpEvent(void* self, char* key)
{
    Desktop* desktop =
    (Desktop*)((char*)self - offsetof(Desktop, keyboardEventHandler));
    desktop->OnKeyUp(desktop, key);
}