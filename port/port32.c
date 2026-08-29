#include "port.h"


/* --------------------------------------------------------------- */
/* Port32Bit Write                                                  */
/* --------------------------------------------------------------- */

static void Port32Bit_Write_impl(Port32Bit* self, uint32_t data)
{
    asm volatile(
        "outl %0, %1"
        :
        : "a"(data), "Nd"(self->base.portnumber)
    );
}


/* --------------------------------------------------------------- */
/* Port32Bit Read                                                   */
/* --------------------------------------------------------------- */

static uint32_t Port32Bit_Read_impl(Port32Bit* self)
{
    uint32_t result;

    asm volatile(
        "inl %1, %0"
        : "=a"(result)
        : "Nd"(self->base.portnumber)
    );

    return result;
}


/* --------------------------------------------------------------- */
/* Port32Bit Destroy                                                */
/* --------------------------------------------------------------- */

static void Port32Bit_Destroy_impl(Port32Bit* self)
{
    (void)self;
}


/* --------------------------------------------------------------- */
/* Port32Bit VTable                                                  */
/* --------------------------------------------------------------- */

static const Port32BitVTable port32bit_vtable = {
    .Write   = Port32Bit_Write_impl,
    .Read    = Port32Bit_Read_impl,
    .Destroy = Port32Bit_Destroy_impl
};


/* --------------------------------------------------------------- */
/* Port32Bit Initialization                                         */
/* --------------------------------------------------------------- */

void Port32Bit_init(Port32Bit* self, uint16_t portnumber)
{
    Port_init(&self->base, portnumber);

    self->vtable = &port32bit_vtable;
}


/* --------------------------------------------------------------- */
/* Public Write                                                     */
/* --------------------------------------------------------------- */

void Port32Bit_Write(Port32Bit* self, uint32_t data)
{
    self->vtable->Write(self, data);
}


/* --------------------------------------------------------------- */
/* Public Read                                                      */
/* --------------------------------------------------------------- */

uint32_t Port32Bit_Read(Port32Bit* self)
{
    return self->vtable->Read(self);
}


/* --------------------------------------------------------------- */
/* Public Destroy                                                   */
/* --------------------------------------------------------------- */

void Port32Bit_destroy(Port32Bit* self)
{
    self->vtable->Destroy(self);

    Port_deinit(&self->base);
}