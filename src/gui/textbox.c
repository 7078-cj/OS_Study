#include "gui/textbox.h"
#include "gui/font8x8.h"
#include "driver/vga.h"

#define CHAR_W 8
#define CHAR_H 8
#define PAD    2                /* space between border and text */


/* True if this text box currently holds the keyboard focus.
 * The parent is always a CompositeWidget in practice, and its first fields
 * match Widget's, so the cast is safe (that is the layout rule again). */
static bool TextBox_hasFocus(TextBox *tb)
{
    Widget *parent = tb->widget.parent;

    if (parent == 0)
        return false;

    return ((CompositeWidget *)parent)->focussedChild == &tb->widget;
}


/* Draw one 8x8 character. Bit 0 of each row byte is the LEFTMOST pixel. */
static void TextBox_drawChar(GraphicsContext *gc, int32_t x, int32_t y, char c)
{
    unsigned char ch = (unsigned char)c;

    if (ch >= 128)
        return;

    const unsigned char *glyph = font8x8_basic[ch];

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            if (glyph[row] & (1 << col))
                PutPixel(gc, x + col, y + row, 0x00, 0x00, 0x00);   /* black text */
        }
    }
}


void TextBox_Init(
    TextBox *self,
    Widget *parent,
    KeyboardDriver *keyboard,
    MouseDriver *mouse,
    int32_t x,
    int32_t y,
    int32_t w,
    int32_t h
)
{
    /* White background. Widget_init also sets Focussable = true and
     * OnMouseDown = Widget_onMouseDown, which asks for focus when clicked. */
    Widget_init(&self->widget, parent, keyboard, mouse, x, y, w, h, 0xFF, 0xFF, 0xFF);

    /* Override only what is different from a plain Widget. */
    self->widget.Draw      = TextBox_draw;
    self->widget.OnKeyDown = TextBox_onKeyDown;

    self->text[0] = '\0';
    self->length  = 0;
}


void TextBox_draw(void *self, GraphicsContext *gc)
{
    TextBox *tb = (TextBox *)self;
    Widget  *w  = &tb->widget;

    int32_t X = 0;
    int32_t Y = 0;
    w->ModelToScreen(w, &X, &Y);

    bool focused = TextBox_hasFocus(tb);

    /* Border: blue when focused, black otherwise. */
    if (focused)
        FillRectangle(gc, X, Y, w->w, w->h, 0x00, 0x00, 0xA8);
    else
        FillRectangle(gc, X, Y, w->w, w->h, 0x00, 0x00, 0x00);

    /* Inside. */
    FillRectangle(gc, X + 1, Y + 1, w->w - 2, w->h - 2, w->r, w->g, w->b);

    /* Only the last `visible` characters fit, so scroll to the end. */
    int visible = (w->w - 2 * PAD) / CHAR_W;
    int start   = (tb->length > visible) ? tb->length - visible : 0;
    int32_t textY = Y + (w->h - CHAR_H) / 2;

    for (int i = start; i < tb->length; i++)
        TextBox_drawChar(gc, X + PAD + (i - start) * CHAR_W, textY, tb->text[i]);

    /* Cursor: a thin bar after the last character (only when focused). */
    if (focused)
    {
        int32_t cx = X + PAD + (tb->length - start) * CHAR_W;
        FillRectangle(gc, cx, Y + PAD, 1, w->h - 2 * PAD, 0x00, 0x00, 0x00);
    }
}


void TextBox_onKeyDown(void *self, char *str)
{
    TextBox *tb = (TextBox *)self;

    for (int i = 0; str[i] != '\0'; i++)
    {
        char c = str[i];

        if (c == '\b')                                  /* backspace */
        {
            if (tb->length > 0)
                tb->text[--tb->length] = '\0';
        }
        else if (c >= 32 && c < 127)                    /* printable */
        {
            if (tb->length < TEXTBOX_MAX)
            {
                tb->text[tb->length++] = c;
                tb->text[tb->length]   = '\0';
            }
        }
        /* '\n' (Enter) is ignored for now. */
    }
}