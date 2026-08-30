#include "keyboard.h"

void KeyboardDriver_init(KeyboardDriver* self, InterruptManager* im){
    self->shift = 0;
    InterruptHandler_Init(&self->interruptHandler, 0x21, im);

    self->interruptHandler.HandleInterrupt = &KeyboardDriver_HandleInterrupt;

    Port8Bit_init(&self->dataport, 0x60);
    Port8Bit_init(&self->commandPort, 0x64);


    while (Port8Bit_Read(&self->commandPort) & 0x1)
    {
        Port8Bit_Read(&self->dataport);
    }

    Port8Bit_Write(&self->commandPort, 0xAE); // activate  keyboard communication
    Port8Bit_Write(&self->commandPort, 0x20); // get the current state

    uint8_t status = (Port8Bit_Read(&self->dataport) | 1) & ~0x10;

    Port8Bit_Write(&self->commandPort, 0x60);
    
    Port8Bit_Write(&self->dataport, status);

    Port8Bit_Write(&self->dataport, 0xF4);

    


}

void printf(char* str);

uint32_t KeyboardDriver_HandleInterrupt(
    void *self,
    uint32_t esp
)
{
    KeyboardDriver *keyboard = (KeyboardDriver *)self;

    uint8_t key = Port8Bit_Read(&keyboard->dataport);

    /* Left Shift pressed */
    if (key == 0x2A)
    {
        keyboard->shift = 1;
        return esp;
    }

    /* Right Shift pressed */
    if (key == 0x36)
    {
        keyboard->shift = 1;
        return esp;
    }

    /* Left Shift released */
    if (key == 0xAA)
    {
        keyboard->shift = 0;
        return esp;
    }

    /* Right Shift released */
    if (key == 0xB6)
    {
        keyboard->shift = 0;
        return esp;
    }

    /* Ignore key releases */
    if (key >= 0x80)
        return esp;

    if (keyboard->shift)
    {
        switch (key)
        {
            /* Numbers */
            case 0x02: printf("!"); break;
            case 0x03: printf("@"); break;
            case 0x04: printf("#"); break;
            case 0x05: printf("$"); break;
            case 0x06: printf("%"); break;
            case 0x07: printf("^"); break;
            case 0x08: printf("&"); break;
            case 0x09: printf("*"); break;
            case 0x0A: printf("("); break;
            case 0x0B: printf(")"); break;

            /* Letters */
            case 0x10: printf("Q"); break;
            case 0x11: printf("W"); break;
            case 0x12: printf("E"); break;
            case 0x13: printf("R"); break;
            case 0x14: printf("T"); break;
            case 0x15: printf("Y"); break;
            case 0x16: printf("U"); break;
            case 0x17: printf("I"); break;
            case 0x18: printf("O"); break;
            case 0x19: printf("P"); break;

            case 0x1E: printf("A"); break;
            case 0x1F: printf("S"); break;
            case 0x20: printf("D"); break;
            case 0x21: printf("F"); break;
            case 0x22: printf("G"); break;
            case 0x23: printf("H"); break;
            case 0x24: printf("J"); break;
            case 0x25: printf("K"); break;
            case 0x26: printf("L"); break;

            case 0x2C: printf("Z"); break;
            case 0x2D: printf("X"); break;
            case 0x2E: printf("C"); break;
            case 0x2F: printf("V"); break;
            case 0x30: printf("B"); break;
            case 0x31: printf("N"); break;
            case 0x32: printf("M"); break;

            /* Symbols */
            case 0x0C: printf("_"); break;
            case 0x0D: printf("+"); break;
            case 0x1A: printf("{"); break;
            case 0x1B: printf("}"); break;
            case 0x27: printf(":"); break;
            case 0x28: printf("\""); break;
            case 0x29: printf("~"); break;
            case 0x2B: printf("|"); break;
            case 0x33: printf("<"); break;
            case 0x34: printf(">"); break;
            case 0x35: printf("?"); break;

            default:
                break;
        }
    }
    else
    {
        switch (key)
        {
            /* Number row */
            case 0x02: printf("1"); break;
            case 0x03: printf("2"); break;
            case 0x04: printf("3"); break;
            case 0x05: printf("4"); break;
            case 0x06: printf("5"); break;
            case 0x07: printf("6"); break;
            case 0x08: printf("7"); break;
            case 0x09: printf("8"); break;
            case 0x0A: printf("9"); break;
            case 0x0B: printf("0"); break;

            /* Letters */
            case 0x10: printf("q"); break;
            case 0x11: printf("w"); break;
            case 0x12: printf("e"); break;
            case 0x13: printf("r"); break;
            case 0x14: printf("t"); break;
            case 0x15: printf("y"); break;
            case 0x16: printf("u"); break;
            case 0x17: printf("i"); break;
            case 0x18: printf("o"); break;
            case 0x19: printf("p"); break;

            case 0x1E: printf("a"); break;
            case 0x1F: printf("s"); break;
            case 0x20: printf("d"); break;
            case 0x21: printf("f"); break;
            case 0x22: printf("g"); break;
            case 0x23: printf("h"); break;
            case 0x24: printf("j"); break;
            case 0x25: printf("k"); break;
            case 0x26: printf("l"); break;

            case 0x2C: printf("z"); break;
            case 0x2D: printf("x"); break;
            case 0x2E: printf("c"); break;
            case 0x2F: printf("v"); break;
            case 0x30: printf("b"); break;
            case 0x31: printf("n"); break;
            case 0x32: printf("m"); break;

            /* Symbols */
            case 0x0C: printf("-"); break;
            case 0x0D: printf("="); break;
            case 0x1A: printf("["); break;
            case 0x1B: printf("]"); break;
            case 0x27: printf(";"); break;
            case 0x28: printf("'"); break;
            case 0x29: printf("`"); break;
            case 0x2B: printf("\\"); break;
            case 0x33: printf(","); break;
            case 0x34: printf("."); break;
            case 0x35: printf("/"); break;

            /* Special */
            case 0x01: printf("ESC"); break;
            case 0x0E: printf("BACKSPACE"); break;
            case 0x0F: printf("TAB"); break;
            case 0x1C: printf("ENTER"); break;
            case 0x39: printf(" "); break;

            default:
                break;
        }
    }

    return esp;
}