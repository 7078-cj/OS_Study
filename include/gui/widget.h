#ifndef __WIDGET_H            
#define __WIDGET_H  

#include "common/types.h"
#include "common/graphicscontext.h"

typedef struct Widget Widget;


typedef struct Widget
{
    Widget* parent;
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
    void (*OnMouseDown)(void* self, int32_t x, int32_t y);
    void (*OnMouseUp)(void* self, int32_t x, int32_t y);
    void (*OnMouseMove)(void* self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);
    void (*OnKeyDown)(void* self, char* str);
    void (*OnKeyUp)(void* self, char* str);

} Widget;

void Widget_init(    
    Widget *self, 
    Widget *parent, 
    int32_t x,
    int32_t y,
    int32_t w,
    int32_t h,
    uint8_t r,
    uint8_t g,
    uint8_t b);
void Widget_deActivate(void *self);

void Widget_getFocus(void *self);
void Widget_ModelToScreen(void *self, int32_t* x, int32_t* y);

void Widget_draw(void *self, GraphicsContext *gc);

void Widget_onMouseDown(void *self, int32_t x, int32_t y);
void Widget_onMouseUp(void *self, int32_t x, int32_t y);
void Widget_onMouseMove(void *self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);

void Widget_onKeyDown(void *self, char* str);
void Widget_onKeyUp(void *self, char* str);

typedef struct  CompositeWidget
{
    /* data */
    Widget* parent;
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
    void (*OnMouseDown)(void* self, int32_t x, int32_t y);
    void (*OnMouseUp)(void* self, int32_t x, int32_t y);
    void (*OnMouseMove)(void* self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);
    void (*OnKeyDown)(void* self, char* str);
    void (*OnKeyUp)(void* self, char* str);

} CompositeWidget;

void CompositeWidget_init(
    CompositeWidget *self,
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

void CompositeWidget_getFocus(void *self);
void CompositeWidget_ModelToScreen(void *self, int32_t* x, int32_t* y);

void CompositeWidget_draw(void *self, GraphicsContext *gc);

void CompositeWidget_onMouseDown(void *self, int32_t x, int32_t y);
void CompositeWidget_onMouseUp(void *self, int32_t x, int32_t y);
void CompositeWidget_onMouseMove(void *self, int32_t old_x, int32_t old_y, int32_t new_x, int32_t new_y);

void CompositeWidget_onKeyDown(void *self, char* str);
void CompositeWidget_onKeyUp(void *self, char* str);



#endif