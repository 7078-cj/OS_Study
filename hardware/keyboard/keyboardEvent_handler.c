#include "keyboard.h"

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

void onKeyDown(char* key)
{
    printf(key);
}

void KeyboardEventHandler_onKeyDown(void* self, uint8_t key){
    KeyboardDriver *keyboard = (KeyboardDriver *)self;

    

    if (keyboard->shift == 1)
    {
        switch (key)
        {
            /* Numbers */
            case 0x02: onKeyDown("!"); break;
            case 0x03: onKeyDown("@"); break;
            case 0x04: onKeyDown("#"); break;
            case 0x05: onKeyDown("$"); break;
            case 0x06: onKeyDown("%"); break;
            case 0x07: onKeyDown("^"); break;
            case 0x08: onKeyDown("&"); break;
            case 0x09: onKeyDown("*"); break;
            case 0x0A: onKeyDown("("); break;
            case 0x0B: onKeyDown(")"); break;

            /* Letters */
            case 0x10: onKeyDown("Q"); break;
            case 0x11: onKeyDown("W"); break;
            case 0x12: onKeyDown("E"); break;
            case 0x13: onKeyDown("R"); break;
            case 0x14: onKeyDown("T"); break;
            case 0x15: onKeyDown("Y"); break;
            case 0x16: onKeyDown("U"); break;
            case 0x17: onKeyDown("I"); break;
            case 0x18: onKeyDown("O"); break;
            case 0x19: onKeyDown("P"); break;

            case 0x1E: onKeyDown("A"); break;
            case 0x1F: onKeyDown("S"); break;
            case 0x20: onKeyDown("D"); break;
            case 0x21: onKeyDown("F"); break;
            case 0x22: onKeyDown("G"); break;
            case 0x23: onKeyDown("H"); break;
            case 0x24: onKeyDown("J"); break;
            case 0x25: onKeyDown("K"); break;
            case 0x26: onKeyDown("L"); break;

            case 0x2C: onKeyDown("Z"); break;
            case 0x2D: onKeyDown("X"); break;
            case 0x2E: onKeyDown("C"); break;
            case 0x2F: onKeyDown("V"); break;
            case 0x30: onKeyDown("B"); break;
            case 0x31: onKeyDown("N"); break;
            case 0x32: onKeyDown("M"); break;

            /* Symbols */
            case 0x0C: onKeyDown("_"); break;
            case 0x0D: onKeyDown("+"); break;
            case 0x1A: onKeyDown("{"); break;
            case 0x1B: onKeyDown("}"); break;
            case 0x27: onKeyDown(":"); break;
            case 0x28: onKeyDown("\""); break;
            case 0x29: onKeyDown("~"); break;
            case 0x2B: onKeyDown("|"); break;
            case 0x33: onKeyDown("<"); break;
            case 0x34: onKeyDown(">"); break;
            case 0x35: onKeyDown("?"); break;

            default:
                break;
        }
    }
    else
    {
        switch (key)
        {
            /* Number row */
            case 0x02: onKeyDown("1"); break;
            case 0x03: onKeyDown("2"); break;
            case 0x04: onKeyDown("3"); break;
            case 0x05: onKeyDown("4"); break;
            case 0x06: onKeyDown("5"); break;
            case 0x07: onKeyDown("6"); break;
            case 0x08: onKeyDown("7"); break;
            case 0x09: onKeyDown("8"); break;
            case 0x0A: onKeyDown("9"); break;
            case 0x0B: onKeyDown("0"); break;

            /* Letters */
            case 0x10: onKeyDown("q"); break;
            case 0x11: onKeyDown("w"); break;
            case 0x12: onKeyDown("e"); break;
            case 0x13: onKeyDown("r"); break;
            case 0x14: onKeyDown("t"); break;
            case 0x15: onKeyDown("y"); break;
            case 0x16: onKeyDown("u"); break;
            case 0x17: onKeyDown("i"); break;
            case 0x18: onKeyDown("o"); break;
            case 0x19: onKeyDown("p"); break;

            case 0x1E: onKeyDown("a"); break;
            case 0x1F: onKeyDown("s"); break;
            case 0x20: onKeyDown("d"); break;
            case 0x21: onKeyDown("f"); break;
            case 0x22: onKeyDown("g"); break;
            case 0x23: onKeyDown("h"); break;
            case 0x24: onKeyDown("j"); break;
            case 0x25: onKeyDown("k"); break;
            case 0x26: onKeyDown("l"); break;

            case 0x2C: onKeyDown("z"); break;
            case 0x2D: onKeyDown("x"); break;
            case 0x2E: onKeyDown("c"); break;
            case 0x2F: onKeyDown("v"); break;
            case 0x30: onKeyDown("b"); break;
            case 0x31: onKeyDown("n"); break;
            case 0x32: onKeyDown("m"); break;

            /* Symbols */
            case 0x0C: onKeyDown("-"); break;
            case 0x0D: onKeyDown("="); break;
            case 0x1A: onKeyDown("["); break;
            case 0x1B: onKeyDown("]"); break;
            case 0x27: onKeyDown(";"); break;
            case 0x28: onKeyDown("'"); break;
            case 0x29: onKeyDown("`"); break;
            case 0x2B: onKeyDown("\\"); break;
            case 0x33: onKeyDown(","); break;
            case 0x34: onKeyDown("."); break;
            case 0x35: onKeyDown("/"); break;

            /* Special */
            case 0x01: onKeyDown("ESC"); break;
            case 0x0E: backspace(); break;
            case 0x0F: onKeyDown("TAB"); break;
            case 0x1C: onKeyDown("ENTER"); break;
            case 0x39: onKeyDown(" "); break;

            default:
                printf("UNKNOWN KEY: ");
                printHex(key);
                break;
        }
    }
}

