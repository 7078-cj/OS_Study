#include "driver/keyboard.h"

void printf(char* str);
extern uint8_t CursorX;
extern uint8_t CursorY;
extern uint16_t* VideoMemory;
extern void printHex(uint8_t key);

void backspace()
{
    if (CursorX == 0)
    {
        if (CursorY == 0)
            return; // already at top-left, nothing to erase

        CursorY--;
        CursorX = 79;
    }
    else
    {
        CursorX--;
    }

    int offset = CursorY * 80 + CursorX;
    VideoMemory[offset] = (VideoMemory[offset] & 0xFF00) | ' ';
}

void onKeyDown(KeyboardDriver* keyboard, char* key)
{

    if (keyboard->handler != 0)
    {
        keyboard->handler->OnKeyDown(keyboard->handler, key);
    }
}

void KeyboardEventHandler_onKeyDown(void* self, uint8_t key){
    KeyboardDriver *keyboard = (KeyboardDriver *)self;

    if (keyboard->shift == 1)
    {
        switch (key)
        {
            /* Numbers */
            case 0x02: onKeyDown(keyboard, "!"); break;
            case 0x03: onKeyDown(keyboard, "@"); break;
            case 0x04: onKeyDown(keyboard, "#"); break;
            case 0x05: onKeyDown(keyboard, "$"); break;
            case 0x06: onKeyDown(keyboard, "%"); break;
            case 0x07: onKeyDown(keyboard, "^"); break;
            case 0x08: onKeyDown(keyboard, "&"); break;
            case 0x09: onKeyDown(keyboard, "*"); break;
            case 0x0A: onKeyDown(keyboard, "("); break;
            case 0x0B: onKeyDown(keyboard, ")"); break;

            /* Letters */
            case 0x10: onKeyDown(keyboard, "Q"); break;
            case 0x11: onKeyDown(keyboard, "W"); break;
            case 0x12: onKeyDown(keyboard, "E"); break;
            case 0x13: onKeyDown(keyboard, "R"); break;
            case 0x14: onKeyDown(keyboard, "T"); break;
            case 0x15: onKeyDown(keyboard, "Y"); break;
            case 0x16: onKeyDown(keyboard, "U"); break;
            case 0x17: onKeyDown(keyboard, "I"); break;
            case 0x18: onKeyDown(keyboard, "O"); break;
            case 0x19: onKeyDown(keyboard, "P"); break;

            case 0x1E: onKeyDown(keyboard, "A"); break;
            case 0x1F: onKeyDown(keyboard, "S"); break;
            case 0x20: onKeyDown(keyboard, "D"); break;
            case 0x21: onKeyDown(keyboard, "F"); break;
            case 0x22: onKeyDown(keyboard, "G"); break;
            case 0x23: onKeyDown(keyboard, "H"); break;
            case 0x24: onKeyDown(keyboard, "J"); break;
            case 0x25: onKeyDown(keyboard, "K"); break;
            case 0x26: onKeyDown(keyboard, "L"); break;

            case 0x2C: onKeyDown(keyboard, "Z"); break;
            case 0x2D: onKeyDown(keyboard, "X"); break;
            case 0x2E: onKeyDown(keyboard, "C"); break;
            case 0x2F: onKeyDown(keyboard, "V"); break;
            case 0x30: onKeyDown(keyboard, "B"); break;
            case 0x31: onKeyDown(keyboard, "N"); break;
            case 0x32: onKeyDown(keyboard, "M"); break;

            /* Symbols */
            case 0x0C: onKeyDown(keyboard, "_"); break;
            case 0x0D: onKeyDown(keyboard, "+"); break;
            case 0x1A: onKeyDown(keyboard, "{"); break;
            case 0x1B: onKeyDown(keyboard, "}"); break;
            case 0x27: onKeyDown(keyboard, ":"); break;
            case 0x28: onKeyDown(keyboard, "\""); break;
            case 0x29: onKeyDown(keyboard, "~"); break;
            case 0x2B: onKeyDown(keyboard, "|"); break;
            case 0x33: onKeyDown(keyboard, "<"); break;
            case 0x34: onKeyDown(keyboard, ">"); break;
            case 0x35: onKeyDown(keyboard, "?"); break;

            default:
                break;
        }
    }
    else
    {
        switch (key)
        {
            /* Number row */
            case 0x02: onKeyDown(keyboard, "1"); break;
            case 0x03: onKeyDown(keyboard, "2"); break;
            case 0x04: onKeyDown(keyboard, "3"); break;
            case 0x05: onKeyDown(keyboard, "4"); break;
            case 0x06: onKeyDown(keyboard, "5"); break;
            case 0x07: onKeyDown(keyboard, "6"); break;
            case 0x08: onKeyDown(keyboard, "7"); break;
            case 0x09: onKeyDown(keyboard, "8"); break;
            case 0x0A: onKeyDown(keyboard, "9"); break;
            case 0x0B: onKeyDown(keyboard, "0"); break;

            /* Letters */
            case 0x10: onKeyDown(keyboard, "q"); break;
            case 0x11: onKeyDown(keyboard, "w"); break;
            case 0x12: onKeyDown(keyboard, "e"); break;
            case 0x13: onKeyDown(keyboard, "r"); break;
            case 0x14: onKeyDown(keyboard, "t"); break;
            case 0x15: onKeyDown(keyboard, "y"); break;
            case 0x16: onKeyDown(keyboard, "u"); break;
            case 0x17: onKeyDown(keyboard, "i"); break;
            case 0x18: onKeyDown(keyboard, "o"); break;
            case 0x19: onKeyDown(keyboard, "p"); break;

            case 0x1E: onKeyDown(keyboard, "a"); break;
            case 0x1F: onKeyDown(keyboard, "s"); break;
            case 0x20: onKeyDown(keyboard, "d"); break;
            case 0x21: onKeyDown(keyboard, "f"); break;
            case 0x22: onKeyDown(keyboard, "g"); break;
            case 0x23: onKeyDown(keyboard, "h"); break;
            case 0x24: onKeyDown(keyboard, "j"); break;
            case 0x25: onKeyDown(keyboard, "k"); break;
            case 0x26: onKeyDown(keyboard, "l"); break;

            case 0x2C: onKeyDown(keyboard, "z"); break;
            case 0x2D: onKeyDown(keyboard, "x"); break;
            case 0x2E: onKeyDown(keyboard, "c"); break;
            case 0x2F: onKeyDown(keyboard, "v"); break;
            case 0x30: onKeyDown(keyboard, "b"); break;
            case 0x31: onKeyDown(keyboard, "n"); break;
            case 0x32: onKeyDown(keyboard, "m"); break;

            /* Symbols */
            case 0x0C: onKeyDown(keyboard, "-"); break;
            case 0x0D: onKeyDown(keyboard, "="); break;
            case 0x1A: onKeyDown(keyboard, "["); break;
            case 0x1B: onKeyDown(keyboard, "]"); break;
            case 0x27: onKeyDown(keyboard, ";"); break;
            case 0x28: onKeyDown(keyboard, "'"); break;
            case 0x29: onKeyDown(keyboard, "`"); break;
            case 0x2B: onKeyDown(keyboard, "\\"); break;
            case 0x33: onKeyDown(keyboard, ","); break;
            case 0x34: onKeyDown(keyboard, "."); break;
            case 0x35: onKeyDown(keyboard, "/"); break;

            /* Special */
            case 0x01: onKeyDown(keyboard, "ESC"); break;
            case 0x0E: onKeyDown(keyboard, "\b"); break;
            case 0x0F: onKeyDown(keyboard, "TAB"); break;
            case 0x1C: onKeyDown(keyboard, "ENTER"); break;
            case 0x39: onKeyDown(keyboard, " "); break;

            default:
                printf("UNKNOWN KEY: ");
                printHex(key);
                break;
        }
    }
}