#ifndef PORT_H
#define PORT_H

#include "../types.h"   

/* ------------------------------------------------------------------ */
/* Port : base type (was: class Port)                                  */
/* No virtual functions of its own, so no vtable needed here.          */
/* ------------------------------------------------------------------ */
typedef struct Port {
    uint16_t portnumber;
} Port;

void Port_init(Port *self, uint16_t portnumber);
void Port_deinit(Port *self);

/* ------------------------------------------------------------------ */
/* Port8Bit : public Port                                              */
/* Has virtual Write/Read, so it carries a vtable pointer.             */
/* ------------------------------------------------------------------ */
typedef struct Port8Bit Port8Bit;

typedef struct Port8BitVTable {
    void    (*Write)(Port8Bit *self, uint8_t data);
    uint8_t (*Read)(Port8Bit *self);
    void    (*Destroy)(Port8Bit *self); /* stands in for ~Port8Bit() */
} Port8BitVTable;

struct Port8Bit {
    Port base;                     /* "inherited" members */
    const Port8BitVTable *vtable;  /* dispatch table */
};

Port8Bit *Port8Bit_create(uint16_t portnumber);
void      Port8Bit_destroy(Port8Bit *self);

/* call-through helpers so call sites look like p->Write(data) */
static inline void Port8Bit_Write(Port8Bit *self, uint8_t data) {
    self->vtable->Write(self, data);
}
static inline uint8_t Port8Bit_Read(Port8Bit *self) {
    return self->vtable->Read(self);
}

/* ------------------------------------------------------------------ */
/* Port8BitSlow : public Port8Bit                                      */
/* Overrides Write only, so it reuses Port8Bit's Read via the vtable.  */
/* ------------------------------------------------------------------ */
typedef struct Port8BitSlow {
    Port8Bit base;  /* "inherited" members, incl. base.vtable */
} Port8BitSlow;

Port8BitSlow *Port8BitSlow_create(uint16_t portnumber);
void          Port8BitSlow_destroy(Port8BitSlow *self);

/* ------------------------------------------------------------------ */
/* Port16Bit : public Port                                             */
/* Independent virtual Write/Read pair, own vtable type.               */
/* ------------------------------------------------------------------ */
typedef struct Port16Bit Port16Bit;

typedef struct Port16BitVTable {
    void     (*Write)(Port16Bit *self, uint16_t data);
    uint16_t (*Read)(Port16Bit *self);
    void     (*Destroy)(Port16Bit *self);
} Port16BitVTable;

struct Port16Bit {
    Port base;
    const Port16BitVTable *vtable;
};

Port16Bit *Port16Bit_create(uint16_t portnumber);
void       Port16Bit_destroy(Port16Bit *self);

static inline void Port16Bit_Write(Port16Bit *self, uint16_t data) {
    self->vtable->Write(self, data);
}
static inline uint16_t Port16Bit_Read(Port16Bit *self) {
    return self->vtable->Read(self);
}

#endif /* PORT_H */