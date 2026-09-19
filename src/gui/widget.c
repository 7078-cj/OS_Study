#include "gui/widget.h"
#include "common/graphicscontext.h"
#include "driver/vga.h"

bool ContainsCoordinate(void* self, int32_t x, int32_t y){
    Widget *w = (Widget *)self;
    return w->x <= x && x < w->x + w->w && w->y <= y && y < w->y + w->h;
}

/* ============================================================
 * Widget
 * ============================================================ */

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
    uint8_t b
)
{
    self->parent = parent;

    self->x = x;
    self->y = y;
    self->w = w;
    self->h = h;

    self->r = r;
    self->g = g;
    self->b = b;

    self->Focussable = true;

    self->GetFocus      = Widget_getFocus;
    self->ModelToScreen = Widget_ModelToScreen;
    self->Draw          = Widget_draw;

    self->OnMouseDown   = Widget_onMouseDown;
    self->OnMouseUp     = Widget_onMouseUp;
    self->OnMouseMove   = Widget_onMouseMove;

    self->OnKeyDown     = Widget_onKeyDown;
    self->OnKeyUp       = Widget_onKeyUp;
    self->keyboard = keyboard;
    self->mouse = mouse;
}


void Widget_deActivate(void *self)
{
    /* Nothing to do for now */
}


void Widget_getFocus(void *self, Widget *widget)
{
    Widget *widgetSelf = (Widget *)self;

    if (widgetSelf->parent != 0)
    {
        /*
         * Tell the parent that this widget wants focus.
         */
        widgetSelf->parent->GetFocus(
            widgetSelf->parent,
            widget
        );
    }
}


void Widget_ModelToScreen(
    void *self,
    int32_t *x,
    int32_t *y
)
{
    Widget *widget = (Widget *)self;

    /*
     * First convert the coordinates through the parent.
     */
    if (widget->parent != 0)
    {
        widget->parent->ModelToScreen(
            widget->parent,
            x,
            y
        );
    }

    /*
     * Add this widget's local position.
     *
     * IMPORTANT:
     * x and y are pointers, so use *x and *y.
     */
    *x += widget->x;
    *y += widget->y;
}


void Widget_draw(
    void *self,
    GraphicsContext *gc
)
{
    Widget *widget = (Widget *)self;

    int32_t X = 0;
    int32_t Y = 0;

    /*
     * Convert local coordinates to screen coordinates.
     */
    widget->ModelToScreen(
        widget,
        &X,
        &Y
    );

    FillRectangle(
        gc,
        X,
        Y,
        widget->w,
        widget->h,
        widget->r,
        widget->g,
        widget->b
    );
}


void Widget_onMouseDown(
    void *self,
    int32_t x,
    int32_t y,
    uint8_t button
)
{
    Widget *widget = (Widget *)self;

    if (widget->Focussable)
    {
        widget->GetFocus(
            widget,
            widget
        );
    }

    (void)button;
}


void Widget_onMouseUp(
    void *self,
    int32_t x,
    int32_t y,
    uint8_t button
)
{
    (void)self;
    (void)x;
    (void)y;
    (void)button;
}


void Widget_onMouseMove(
    void *self,
    int32_t old_x,
    int32_t old_y,
    int32_t new_x,
    int32_t new_y
)
{
    (void)self;
    (void)old_x;
    (void)old_y;
    (void)new_x;
    (void)new_y;
}


void Widget_onKeyDown(
    void *self,
    char* str
)
{
    (void)self;
    (void)str;
}


void Widget_onKeyUp(
    void *self,
    char* str
)
{
    (void)self;
    (void)str;
}


/* ============================================================
 * CompositeWidget
 * ============================================================ */

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
)
{
    self->parent = parent;

    self->x = x;
    self->y = y;
    self->w = w;
    self->h = h;

    self->r = r;
    self->g = g;
    self->b = b;

    self->Focussable = true;

    self->numChildren = 0;
    self->focussedChild = 0;

    self->GetFocus      = CompositeWidget_getFocus;
    self->ModelToScreen = CompositeWidget_ModelToScreen;
    self->Draw          = CompositeWidget_draw;

    self->OnMouseDown   = CompositeWidget_onMouseDown;
    self->OnMouseUp     = CompositeWidget_onMouseUp;
    self->OnMouseMove   = CompositeWidget_onMouseMove;

    self->OnKeyDown     = CompositeWidget_onKeyDown;
    self->OnKeyUp       = CompositeWidget_onKeyUp;
    self->keyboard = keyboard;
    self->mouse = mouse;
}


void CompositeWidget_deActivate(void *self)
{
    (void)self;
}



void CompositeWidget_getFocus(
    void *self,
    Widget *widget
)
{
    CompositeWidget *composite = (CompositeWidget *)self;

    composite->focussedChild = widget;


    if (composite->parent != 0)
    {
        composite->parent->GetFocus(
            composite->parent,
            widget
        );
    }
}




void CompositeWidget_ModelToScreen(
    void *self,
    int32_t *x,
    int32_t *y
)
{
    CompositeWidget *composite =
        (CompositeWidget *)self;

    if (composite->parent != 0)
    {
        composite->parent->ModelToScreen(
            composite->parent,
            x,
            y
        );
    }

    *x += composite->x;
    *y += composite->y;
}


void CompositeWidget_draw(
    void *self,
    GraphicsContext *gc
)
{
    CompositeWidget *composite =
        (CompositeWidget *)self;

    int32_t X = 0;
    int32_t Y = 0;

    composite->ModelToScreen(
        composite,
        &X,
        &Y
    );

    FillRectangle(
        gc,
        X,
        Y,
        composite->w,
        composite->h,
        composite->r,
        composite->g,
        composite->b
    );

    for (int i = 0; i < composite->numChildren; i++)
    {
        if (composite->children[i] != 0)
        {
            composite->children[i]->Draw(
                composite->children[i],
                gc
            );
        }
    }
}




void CompositeWidget_onMouseDown(
    void *self,
    int32_t x,
    int32_t y,
    uint8_t button
)
{
    CompositeWidget *composite =
        (CompositeWidget *)self;


    for (int i = 0; i < composite->numChildren; i++)
    {
        if(ContainsCoordinate(composite->children[i], x - composite->x, y - composite->y)){
            composite->children[i]->OnMouseDown(composite->children[i], x - composite->x, y - composite->y, button);
            break;
        }
    }
}


void CompositeWidget_onMouseUp(
    void *self,
    int32_t x,
    int32_t y,
    uint8_t button
)
{
    CompositeWidget *composite =
        (CompositeWidget *)self;

    for (int i = 0; i < composite->numChildren; i++)
    {
        if(ContainsCoordinate(composite->children[i], x - composite->x, y - composite->y)){
            composite->children[i]->OnMouseUp(composite->children[i], x - composite->x, y - composite->y, button);
            break;
        }
    }
}


void CompositeWidget_onMouseMove(
    void *self,
    int32_t old_x,
    int32_t old_y,
    int32_t new_x,
    int32_t new_y
)
{
    CompositeWidget *composite =
        (CompositeWidget *)self;

    int firstChild = -1;

    for (int i = 0; i < composite->numChildren; i++)
    {
        if(ContainsCoordinate(composite->children[i], old_x - composite->x, old_y - composite->y)){
            composite->children[i]->OnMouseMove(
                composite->children[i], 
                old_x - composite->x,
                old_y - composite->y, 
                new_x - composite->x, 
                new_y - composite->y
            );
            firstChild = i;
            break;
        }
    }

    for (int i = 0; i < composite->numChildren; i++)
    {
        if(ContainsCoordinate(composite->children[i], new_x - composite->x, new_y - composite->y)){

            if(firstChild != i){
                composite->children[i]->OnMouseMove(
                    composite->children[i], 
                    old_x - composite->x,
                    old_y - composite->y, 
                    new_x - composite->x, 
                    new_y - composite->y
                );
            }
            
            break;
        }
    }
}



void CompositeWidget_onKeyDown(
    void *self,
    char* str
)
{
    CompositeWidget *composite =
        (CompositeWidget *)self;

    if (composite->focussedChild != 0)
    {
        composite->focussedChild->OnKeyDown(composite->focussedChild, str);
    }
}


void CompositeWidget_onKeyUp(
    void *self,
    char* str
)
{
    CompositeWidget *composite =
        (CompositeWidget *)self;

    if (composite->focussedChild != 0)
    {
        composite->focussedChild->OnKeyUp(composite->focussedChild, str);
    }
}

bool CompositeWidget_addChild(void *self, Widget *child){

    CompositeWidget *composite =
        (CompositeWidget *)self;

    if(composite->numChildren >= 100){
        return false;
    }
    composite->children[composite->numChildren++] = child;
    return true;
}