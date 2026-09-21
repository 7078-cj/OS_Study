#include "gui/window.h"
bool redraw;

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
){
    CompositeWidget_init(&self->compositeWidget, keyboard, mouse, parent, x, y, w, h, r, g, b);
    self->dragging = false;
    self->compositeWidget.OnMouseDown = Window_onMouseDown;
    self->compositeWidget.OnMouseUp   = Window_onMouseUp;
    self->compositeWidget.OnMouseMove = Window_onMouseMove;
}


void Window_onMouseDown(void *self, int32_t x, int32_t y, uint8_t button){
    Window *window = (Window *)self;

    window->dragging = (button == 1);
    CompositeWidget_onMouseDown(&window->compositeWidget, x, y, button);
}

void Window_onMouseUp(void *self, int32_t x, int32_t y, uint8_t button){
    Window *window = (Window *)self;

    window->dragging = false;
    CompositeWidget_onMouseUp(&window->compositeWidget, x, y, button);
}

void Window_onMouseMove(void *self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y){
    Window *window = (Window *)self;

    if (window->dragging) {
        window->compositeWidget.x += new_x - old_x;
        window->compositeWidget.y += new_y - old_y;
    }
    CompositeWidget_onMouseMove(&window->compositeWidget, old_x, old_y, new_x, new_y);
}

