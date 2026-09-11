#ifndef __VGA_H
#define __VGA_H

#include "../common/types.h"
#include "../hardwarecommunication/port.h"
#include "../hardwarecommunication/interrupts.h"
#include "driver.h"

typedef struct VideoGraphicsArray {
    Driver driver;
    Port8Bit miscPort;
    Port8Bit crtcIndexPort;
    Port8Bit crtcDataPort;
    Port8Bit sequencerIndexPort;
    Port8Bit sequencerDataPort;
    Port8Bit graphicsControllerIndexPort;
    Port8Bit graphicsControllerDataPort;
    Port8Bit attributeControllerIndexPort;
    Port8Bit attributeControllerReadPort;
    Port8Bit attributeControllerWritePort;
    Port8Bit attributeControllerResetPort;

    uint8_t* framebuffer;

} VideoGraphicsArray;

void VideoGraphicsArray_init(VideoGraphicsArray* self);
void VideoGraphicsArray_deActivate(VideoGraphicsArray* self);

void VideoGraphicsArray_writeRegisters(VideoGraphicsArray* self, uint8_t* registers);
void VideoGraphicsArray_readRegisters(VideoGraphicsArray* self, uint8_t* registers);

uint8_t* VideoGraphicsArray_getFrameBufferSegment(VideoGraphicsArray* self);

bool VideoGraphicsArray_supportsMode(VideoGraphicsArray* self, uint32_t width, uint32_t height, uint32_t colorDepth);
bool VideoGraphicsArray_setMode(VideoGraphicsArray* self, uint32_t width, uint32_t height, uint32_t colorDepth);

void VideoGraphicsArray_putPixel(VideoGraphicsArray* self, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);

uint8_t VideoGraphicsArray_getColorIndex(VideoGraphicsArray* self, uint8_t r, uint8_t g, uint8_t b);

void PutPixel(VideoGraphicsArray* self, uint32_t x, uint32_t y, uint8_t colorIndex);
void FillRectangle(VideoGraphicsArray* self,  uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t r, uint8_t g, uint8_t b);

#endif