#include "port.h"


/* --------------------------------------------------------------- */
/* Port16Bit Write                                                  */
/* --------------------------------------------------------------- */

static void Port16Bit_Write_impl(Port16Bit* self, uint16_t data)
{
    asm volatile(
        "outw %0, %1"
        :
        : "a"(data), "Nd"(self->base.portnumber)
    );
}


/* --------------------------------------------------------------- */
/* Port16Bit Read                                                   */
/* --------------------------------------------------------------- */

static uint16_t Port16Bit_Read_impl(Port16Bit* self)
{
    uint16_t result;

    asm volatile(
        "inw %1, %0"
        : "=a"(result)
        : "Nd"(self->base.portnumber)
    );

    return result;
}


/* --------------------------------------------------------------- */
/* Port16Bit Destroy                                                */
/* --------------------------------------------------------------- */

static void Port16Bit_Destroy_impl(Port16Bit* self)
{
    (void)self;
}


/* --------------------------------------------------------------- */
/* Port16Bit VTable                                                  */
/* --------------------------------------------------------------- */

static const Port16BitVTable port16bit_vtable = {
    .Write   = Port16Bit_Write_impl,
    .Read    = Port16Bit_Read_impl,
    .Destroy = Port16Bit_Destroy_impl
};


/* --------------------------------------------------------------- */
/* Port16Bit Initialization                                         */
/* --------------------------------------------------------------- */

void Port16Bit_init(Port16Bit* self, uint16_t portnumber)
{
    Port_init(&self->base, portnumber);

    self->vtable = &port16bit_vtable;
}


/* --------------------------------------------------------------- */
/* Public Write                                                     */
/* --------------------------------------------------------------- */

void Port16Bit_Write(Port16Bit* self, uint16_t data)
{
    self->vtable->Write(self, data);
}


/* --------------------------------------------------------------- */
/* Public Read                                                      */
/* --------------------------------------------------------------- */

uint16_t Port16Bit_Read(Port16Bit* self)
{
    return self->vtable->Read(self);
}


/* --------------------------------------------------------------- */
/* Public Destroy                                                   */
/* --------------------------------------------------------------- */

void Port16Bit_destroy(Port16Bit* self)
{
    self->vtable->Destroy(self);

    Port_deinit(&self->base);
}