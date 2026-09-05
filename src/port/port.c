#include "port8.c"
#include "port8Slow.c"
#include "port16.c"
#include "port32.c"
#include "hardwarecommunication/port.h"

void Port_init(Port *self, uint16_t portnumber){
    self->portnumber = portnumber;
}

void Port_deinit(Port *self){

}

