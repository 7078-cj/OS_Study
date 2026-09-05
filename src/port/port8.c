#include "hardwarecommunication/port.h"


/* --------------------------------------------------------------- */
/* Port8Bit Write                                                   */
/* --------------------------------------------------------------- */

static void Port8Bit_Write_impl(Port8Bit* self, uint8_t data)
{
    asm volatile(
        "outb %0, %1"
        :
        : "a"(data), "Nd"(self->base.portnumber)
    );
}


/* --------------------------------------------------------------- */
/* Port8Bit Read                                                    */
/* --------------------------------------------------------------- */

static uint8_t Port8Bit_Read_impl(Port8Bit* self)
{
    uint8_t result;

    asm volatile(
        "inb %1, %0"
        : "=a"(result)
        : "Nd"(self->base.portnumber)
    );

    return result;
}


/* --------------------------------------------------------------- */
/* Port8Bit Destroy                                                 */
/* --------------------------------------------------------------- */

static void Port8Bit_Destroy_impl(Port8Bit* self)
{
    /*
     * Nothing to destroy.
     *
     * Port8Bit is not dynamically allocated.
     * The caller owns the memory.
     */
    (void)self;
}


/* --------------------------------------------------------------- */
/* Port8Bit VTable                                                   */
/* --------------------------------------------------------------- */

static const Port8BitVTable port8bit_vtable = {
    .Write   = Port8Bit_Write_impl,
    .Read    = Port8Bit_Read_impl,
    .Destroy = Port8Bit_Destroy_impl
};


/* --------------------------------------------------------------- */
/* Port8Bit Initialization                                           */
/* --------------------------------------------------------------- */

void Port8Bit_init(Port8Bit* self, uint16_t portnumber)
{
    Port_init(&self->base, portnumber);

    self->vtable = &port8bit_vtable;
}


/* --------------------------------------------------------------- */
/* Public Write                                                      */
/* --------------------------------------------------------------- */

void Port8Bit_Write(Port8Bit* self, uint8_t data)
{
    self->vtable->Write(self, data);
}


/* --------------------------------------------------------------- */
/* Public Read                                                       */
/* --------------------------------------------------------------- */

uint8_t Port8Bit_Read(Port8Bit* self)
{
    return self->vtable->Read(self);
}


/* --------------------------------------------------------------- */
/* Public Destroy                                                    */
/* --------------------------------------------------------------- */

void Port8Bit_destroy(Port8Bit* self)
{
    self->vtable->Destroy(self);

    Port_deinit(&self->base);
}