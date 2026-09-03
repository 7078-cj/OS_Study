#ifndef DRIVER_H
#define DRIVER_H

#include "types.h"

typedef struct Driver
{
    void* driverData;
    void (*Activate)(void* self);
    void (*DeActivate)(void* self);
    void (*Reset)(void* self);
} Driver;

void Driver_Activate(void* self);      // default no-op, so drivers that don't
void Driver_DeActivate(void* self);    // need Activate/DeActivate still work
void Driver_Reset(void* self);
void Driver_Init(Driver* self);

typedef struct DriverManager
{
    void* drivers[256];
    uint32_t numDrivers;
} DriverManager;

void DriverManager_init(DriverManager* self);
void DriverManager_deActivate(DriverManager* self);
void DriverManager_activate(DriverManager* self);
void DriverManager_addDriver(DriverManager* self, void* driver);

#endif // DRIVER_H