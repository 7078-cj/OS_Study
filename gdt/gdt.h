#ifndef __GDT_H              // if __GDT_H hasn't been defined yet, keep reading this file...
#define __GDT_H               // ...and now mark it as defined, so this file can't be included twice

#include "../types.h"        // bring in uint8_t, uint16_t, uint32_t (small/medium/big number types)

// Describes ONE entry in the GDT (Global Descriptor Table).
// This is the exact byte-for-byte layout the CPU expects to read —
// the fields are in this specific order and size because the CPU hardware demands it.
typedef struct {
    uint16_t limit_lo;        // lower 16 bits of the segment's size limit
    uint16_t base_lo;         // lower 16 bits of the segment's starting address
    uint8_t  base_hi;         // next 8 bits of the starting address (bits 16-23)
    uint8_t  type;            // what kind of segment this is (code, data, permissions, etc.)
    uint8_t  flags_limit_hi;  // top 4 bits of the size limit + 4 extra on/off flag bits, packed together
    uint8_t  base_vhi;        // final 8 bits of the starting address (bits 24-31)
} __attribute__((packed)) SegmentDescriptor;

// __attribute__((packed)) = don't add any hidden padding/gaps between these fields;
// the CPU reads this memory directly, so the layout must match exactly, byte for byte.

// "Constructor": fills in a SegmentDescriptor's scattered fields above,
// given normal, whole-number base/limit/type values.
void SegmentDescriptor_Init(SegmentDescriptor* desc, uint32_t base, uint32_t limit, uint8_t type);

// "Getter": reassembles the scattered base_lo/base_hi/base_vhi fields
// back into one normal 32-bit starting address.
uint32_t SegmentDescriptor_Base(const SegmentDescriptor* desc);

// "Getter": reassembles the scattered limit fields back into one normal 32-bit size.
uint32_t SegmentDescriptor_Limit(const SegmentDescriptor* desc);
// const SegmentDescriptor* = this function promises to only READ desc, never modify it

// The full GDT: a fixed set of four SegmentDescriptor entries, in a required order.
typedef struct {
    SegmentDescriptor nullSegmentSelector;     // entry 0: must be all-zero, required by the CPU, unused otherwise
    SegmentDescriptor unusedSegmentSelector;   // entry 1: reserved/spare, not currently used
    SegmentDescriptor codeSegmentSelector;     // entry 2: describes where the program's CODE lives in memory
    SegmentDescriptor dataSegmentSelector;     // entry 3: describes where the program's DATA lives in memory
} GlobalDescriptorTable;

// Fills in all four entries above with their correct values in one call.
void GlobalDescriptorTable_Init(GlobalDescriptorTable* gdt);

// Returns the byte offset (position) of the code entry within the table —
// this offset is literally the value the CPU wants when loading the code segment register.
uint16_t GlobalDescriptorTable_CodeSegmentSelector(const GlobalDescriptorTable* gdt);

// Same idea, but for the data entry — the offset needed to load the data segment register.
uint16_t GlobalDescriptorTable_DataSegmentSelector(const GlobalDescriptorTable* gdt);

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) GDTDescriptor;


#endif