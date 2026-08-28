
#include "port.h"

void Port_init(Port *self, uint16_t portnumber){
    self->portnumber = portnumber;
}

void Port_deinit(Port *self){

}

static void Port8Bit_Write_impl(Port8Bit *self, uint8_t data) {
    asm volatile("outb (%0), (%1)" : : "a" (data), "Nd" (self->base.portnumber));
}