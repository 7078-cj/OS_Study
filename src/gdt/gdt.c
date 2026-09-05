#include "../../include/gdt.h"

void SegmentDescriptor_Init(
    SegmentDescriptor* desc,
    uint32_t base,
    uint32_t limit,
    uint8_t type
)
{
    desc->base_lo  = base & 0xFFFF;
    desc->base_hi  = (base >> 16) & 0xFF;
    desc->base_vhi = (base >> 24) & 0xFF;
    desc->type = type;

    if (limit <= 0xFFFF) {
        desc->flags_limit_hi = 0x40;
    } else {
        if ((limit & 0xFFF) != 0xFFF) {
            limit = (limit >> 12) - 1;
        } else {
            limit = limit >> 12;
        }

        desc->flags_limit_hi = 0xC0;
    }

    desc->limit_lo = limit & 0xFFFF;
    desc->flags_limit_hi |= (limit >> 16) & 0x0F;
}

uint32_t SegmentDescriptor_Base(const SegmentDescriptor* desc)
{
    return desc->base_lo |
            ((uint32_t)desc->base_hi << 16) |
            ((uint32_t)desc->base_vhi << 24);
}

uint32_t SegmentDescriptor_Limit(const SegmentDescriptor* desc)
{
    uint32_t limit = desc->limit_lo | ((desc->flags_limit_hi & 0x0F) << 16);

    if ((desc->flags_limit_hi & 0xC0) == 0xC0) {
        limit = (limit << 12) | 0xFFF;
    }

    return limit;
}

void GlobalDescriptorTable_Init(GlobalDescriptorTable* gdt)
{
    SegmentDescriptor_Init(&gdt->nullSegmentSelector, 0, 0, 0);
    SegmentDescriptor_Init(&gdt->unusedSegmentSelector, 0, 0, 0);
    SegmentDescriptor_Init(&gdt->codeSegmentSelector, 0, 64 * 1024 * 1024, 0x9A);
    SegmentDescriptor_Init(&gdt->dataSegmentSelector, 0, 64 * 1024 * 1024, 0x92);

    GDTDescriptor gdtd = {
        .limit = sizeof(GlobalDescriptorTable) - 1,
        .base = (uint32_t)gdt
    };

    asm volatile("lgdt (%0)" : : "p" (&gdtd));
}
uint16_t GlobalDescriptorTable_CodeSegmentSelector(const GlobalDescriptorTable* gdt)
{
    return (uint8_t*)&gdt->codeSegmentSelector - (uint8_t*)gdt;
}

uint16_t GlobalDescriptorTable_DataSegmentSelector(const GlobalDescriptorTable* gdt)
{
    return (uint8_t*)&gdt->dataSegmentSelector - (uint8_t*)gdt;
}