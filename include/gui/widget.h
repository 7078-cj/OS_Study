#ifndef __WIDGET_H            
#define __WIDGET_H  

#include "common/types.h"
#include "common/graphicscontext.h"
#include "driver/keyboard.h"
#include "driver/mouse.h"

typedef struct Widget Widget;


typedef struct Widget
{
    Widget* parent;
    KeyboardDriver *keyboard;
    MouseDriver *mouse;
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    bool Focussable;
    
    void (*GetFocus)(void* self, Widget* widget);
    void (*ModelToScreen)(void* self, int32_t* x, int32_t* y);
    void (*Draw)(void* self, GraphicsContext* gc);
    void (*OnMouseDown)(void* self, int32_t x, int32_t y, uint8_t button);
    void (*OnMouseUp)(void* self, int32_t x, int32_t y, uint8_t button);
    void (*OnMouseMove)(void* self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);
    void (*OnKeyDown)(void* self, char* str);
    void (*OnKeyUp)(void* self, char* str);

} Widget;

void Widget_init(    
    Widget *self, 
    Widget *parent, 
    KeyboardDriver *keyboard,
    MouseDriver *mouse,
    int32_t x,
    int32_t y,
    int32_t w,
    int32_t h,
    uint8_t r,
    uint8_t g,
    uint8_t b);
void Widget_deActivate(void *self);

void Widget_getFocus(void *self, Widget *widget);
void Widget_ModelToScreen(void *self, int32_t* x, int32_t* y);

void Widget_draw(void *self, GraphicsContext *gc);

bool ContainsCoordinate(void *self, int32_t x, int32_t y);

void Widget_onMouseDown(void *self, int32_t x, int32_t y, uint8_t button);
void Widget_onMouseUp(void *self, int32_t x, int32_t y, uint8_t button);
void Widget_onMouseMove(void *self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);

void Widget_onKeyDown(void *self, char* str);
void Widget_onKeyUp(void *self, char* str);

typedef struct  CompositeWidget
{
    /* data */
    Widget* parent;
    KeyboardDriver *keyboard;
    MouseDriver *mouse;
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;

    uint8_t r;
    uint8_t g;
    uint8_t b;
    bool Focussable;
    Widget* children[100];
    int numChildren;
    Widget* focussedChild;

    void (*GetFocus)(void* self, Widget* widget);
    void (*ModelToScreen)(void* self, int32_t* x, int32_t* y);
    void (*Draw)(void* self, GraphicsContext* gc);
    void (*OnMouseDown)(void* self, int32_t x, int32_t y, uint8_t button);
    void (*OnMouseUp)(void* self, int32_t x, int32_t y, uint8_t button);
    void (*OnMouseMove)(void* self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);
    void (*OnKeyDown)(void* self, char* str);
    void (*OnKeyUp)(void* self, char* str);

} CompositeWidget;

void CompositeWidget_init(
    CompositeWidget *self,
    KeyboardDriver *keyboard,
    MouseDriver *mouse,
    Widget *parent, 
    int32_t x,
    int32_t y,
    int32_t w,
    int32_t h,
    uint8_t r,
    uint8_t g,
    uint8_t b
);
void CompositeWidget_deActivate(void *self);

void CompositeWidget_getFocus(void *self, Widget *widget);
void CompositeWidget_ModelToScreen(void *self, int32_t* x, int32_t* y);

void CompositeWidget_draw(void *self, GraphicsContext *gc);
bool CompositeWidget_addChild(void *self, Widget *child);

void CompositeWidget_onMouseDown(void *self, int32_t x, int32_t y, uint8_t button);
void CompositeWidget_onMouseUp(void *self, int32_t x, int32_t y, uint8_t button);
void CompositeWidget_onMouseMove(void *self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);

void CompositeWidget_onKeyDown(void *self, char* str);
void CompositeWidget_onKeyUp(void *self, char* str);



#endif