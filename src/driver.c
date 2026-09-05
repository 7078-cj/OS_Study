#include "driver/driver.h"


void Driver_Activate(void* self)
{
    (void)self;
}

void Driver_DeActivate(void* self)
{
    (void)self;
}

void Driver_Reset(void* self)
{
    (void)self;
}

void Driver_Init(Driver* self)
{
    self->Activate   = &Driver_Activate;
    self->DeActivate = &Driver_DeActivate;
    self->Reset      = &Driver_Reset;
}


void DriverManager_init(DriverManager* self)
{
    self->numDrivers = 0;
}

void DriverManager_addDriver(DriverManager* self, void* driver)
{
    self->drivers[self->numDrivers] = driver;
    self->numDrivers++;
}

void DriverManager_activate(DriverManager* self)
{
    for (uint32_t i = 0; i < self->numDrivers; i++)
        ((Driver*)self->drivers[i])->Activate(self->drivers[i]);
}

void DriverManager_deActivate(DriverManager* self)
{
    for (uint32_t i = 0; i < self->numDrivers; i++)
        ((Driver*)self->drivers[i])->DeActivate(self->drivers[i]);
}