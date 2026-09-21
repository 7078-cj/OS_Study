#ifndef __WINDOW_H            
#define __WINDOW_H  

#include "gui/widget.h"

typedef struct Window
{

    CompositeWidget compositeWidget;
    bool dragging;

} Window;

void Window_Init(
    Window *self, 
    KeyboardDriver *keyboard, 
    MouseDriver *mouse,
    void *parent,
    int32_t x,
    int32_t y,     
    int32_t w,
    int32_t h,
    uint8_t r,
    uint8_t g,
    uint8_t b
);

void Window_onMouseDown(void *self, int32_t x, int32_t y, uint8_t button);
void Window_onMouseUp(void *self, int32_t x, int32_t y, uint8_t button);
void Window_onMouseMove(void *self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);



#endif