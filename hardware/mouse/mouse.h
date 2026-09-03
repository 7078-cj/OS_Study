#ifndef MOUSE_H
#define MOUSE_H

#include "../../types.h"
#include "../../port/port.h"
#include "../../interrupts/interrupts.h"
#include "../../driver.h"

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