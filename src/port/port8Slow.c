#include "hardwarecommunication/port.h"


/* --------------------------------------------------------------- */
/* Port8BitSlow Write                                               */
/* --------------------------------------------------------------- */

static void Port8BitSlow_Write_impl(Port8BitSlow* self, uint8_t data)
{
    asm volatile(
        "outb %0, %1\n"
        "jmp 1f\n"
        "jmp 1f\n"
        "1:"
        :
        : "a"(data), "Nd"(self->base.base.portnumber)
    );
}


/* --------------------------------------------------------------- */
/* Port8BitSlow Initialization                                      */
/* --------------------------------------------------------------- */

void Port8BitSlow_init(Port8BitSlow* self, uint16_t portnumber)
{
    Port8Bit_init(&self->base, portnumber);

    self->Write = Port8BitSlow_Write_impl;
}

/* --------------------------------------------------------------- */
/* Public Destroy                                                   */
/* --------------------------------------------------------------- */

void Port8BitSlow_destroy(Port8BitSlow* self)
{
    self->base.vtable->Destroy(&self->base);
}