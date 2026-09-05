#ifndef __MOUSE_H
#define __MOUSE_H

#include "../common/types.h"
#include "../hardwarecommunication/port.h"
#include "../hardwarecommunication/interrupts.h"
#include "../driver/driver.h"

void MouseEventHandler_onKeyDown(
    void* self
);

void MouseEventHandler_onKeyUp(
    void* self
);


typedef struct MouseDriver
{
    
    InterruptHandler interruptHandler;
    Driver driver;

    Port8Bit dataport;
    Port8Bit commandPort;
    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;


} MouseDriver;

void MouseDriver_init(
    MouseDriver* self,
    InterruptManager* im
);

void MouseDriver_deActivate(
    MouseDriver* self,
    InterruptManager* im
);

uint32_t MouseDriver_HandleInterrupt(
    void* self,
    uint32_t esp
);

#endif