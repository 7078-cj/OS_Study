#ifndef PORT_H
#define PORT_H

#include "../types.h"


/* ------------------------------------------------------------------ */
/* Port : base type                                                    */
/* ------------------------------------------------------------------ */

typedef struct Port {
    uint16_t portnumber;
} Port;

void Port_init(Port* self, uint16_t portnumber);
void Port_deinit(Port* self);


/* ------------------------------------------------------------------ */
/* Port8Bit : public Port                                              */
/* ------------------------------------------------------------------ */

typedef struct Port8Bit Port8Bit;

typedef struct Port8BitVTable {
    void    (*Write)(Port8Bit* self, uint8_t data);
    uint8_t (*Read)(Port8Bit* self);
    void    (*Destroy)(Port8Bit* self);
} Port8BitVTable;

struct Port8Bit {
    Port base;
    const Port8BitVTable* vtable;
};

void Port8Bit_init(Port8Bit* self, uint16_t portnumber);
void Port8Bit_destroy(Port8Bit* self);


/* ------------------------------------------------------------------ */
/* Port8BitSlow : public Port8Bit                                      */
/* ------------------------------------------------------------------ */

typedef struct Port8BitSlow Port8BitSlow;

typedef struct Port8BitSlow {
    Port8Bit base;
    void (*Write)(Port8BitSlow* self, uint8_t data);
} Port8BitSlow;

void Port8BitSlow_init(Port8BitSlow* self, uint16_t portnumber);
void Port8BitSlow_destroy(Port8BitSlow* self);


/* ------------------------------------------------------------------ */
/* Port16Bit : public Port                                             */
/* ------------------------------------------------------------------ */

typedef struct Port16Bit Port16Bit;

typedef struct Port16BitVTable {
    void     (*Write)(Port16Bit* self, uint16_t data);
    uint16_t (*Read)(Port16Bit* self);
    void     (*Destroy)(Port16Bit* self);
} Port16BitVTable;

struct Port16Bit {
    Port base;
    const Port16BitVTable* vtable;
};

void Port16Bit_init(Port16Bit* self, uint16_t portnumber);
void Port16Bit_destroy(Port16Bit* self);

/* ------------------------------------------------------------------ */
/* Port32Bit : public Port                                             */
/* ------------------------------------------------------------------ */

typedef struct Port32Bit Port32Bit;

typedef struct Port32BitVTable {
    void     (*Write)(Port32Bit* self, uint32_t data);
    uint32_t (*Read)(Port32Bit* self);
    void     (*Destroy)(Port32Bit* self);
} Port32BitVTable;

struct Port32Bit {
    Port base;
    const Port32BitVTable* vtable;
};

void Port32Bit_init(Port32Bit* self, uint16_t portnumber);
void Port32Bit_Write(Port32Bit* self, uint32_t data);
uint32_t Port32Bit_Read(Port32Bit* self);
void Port32Bit_destroy(Port32Bit* self);


#endif /* PORT_H */