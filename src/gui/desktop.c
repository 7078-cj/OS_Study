#include "gui/desktop.h"

void Desktop_Init(
    Desktop *self,
    uint32_t MouseX, 
    uint32_t MouseY, 
    KeyboardDriver *keyboard, 
    MouseDriver *mouse,
    int32_t w,
    int32_t h,
    uint8_t r,
    uint8_t g,
    uint8_t b

){
    self->keyboard = keyboard;
    self->mouse = mouse;
    CompositeWidget_init(self->compositeWidget, keyboard, mouse, 0, 0, 0, w, h, r, g, b);
    self->MouseX = w/2;
    self->MouseY = h/2;

    
}
void Desktop_draw(void* self, GraphicsContext *gc){
    
}

void Desktop_onMouseDown(void *self, int32_t x, int32_t y, uint8_t button){
    
}
void Desktop_onMouseUp(void *self, int32_t x, int32_t y, uint8_t button){
    
}
void Desktop_onMouseMove(void *self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y){
    
}