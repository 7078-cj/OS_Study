#ifndef __TEXTBOX_H
#define __TEXTBOX_H

#include "gui/widget.h"

#define TEXTBOX_MAX 64          /* max characters stored */

typedef struct TextBox
{
    Widget widget;              /* MUST be the first member (see notes on struct layout) */

    char text[TEXTBOX_MAX + 1]; /* +1 for the terminating '\0' */
    int  length;
} TextBox;

void TextBox_Init(
    TextBox *self,
    Widget *parent,             /* the window (or desktop) that will contain it */
    KeyboardDriver *keyboard,
    MouseDriver *mouse,
    int32_t x,                  /* position relative to the parent */
    int32_t y,
    int32_t w,
    int32_t h
);

void TextBox_draw(void *self, GraphicsContext *gc);
void TextBox_onKeyDown(void *self, char *str);

#endif