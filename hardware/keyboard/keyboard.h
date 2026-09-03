#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../types.h"
#include "../../port/port.h"
#include "../../interrupts/interrupts.h"
#include "../../driver.h"

void KeyboardEventHandler_onKeyDown(
    void* self,
    uint8_t key
);

void KeyboardEventHandler_onKeyUp(
    void* self,
    uint8_t key
);



typedef struct KeyboardDriver
{
    
    InterruptHandler interruptHandler;
    Driver driver;

    Port8Bit dataport;
    Port8Bit commandPort;
    uint8_t shift;


} KeyboardDriver;

void KeyboardDriver_init(
    KeyboardDriver* self,
    InterruptManager* im
);

void KeyboardDriver_deActivate(
    KeyboardDriver* self,
    InterruptManager* im
);

uint32_t KeyboardDriver_HandleInterrupt(
    void* self,
    uint32_t esp
);

#endif