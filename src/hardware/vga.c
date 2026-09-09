#include "driver/vga.h"
#include "driver/driver.h"

void VideoGraphicsArray_init(VideoGraphicsArray* self){
    Port8Bit_init(&self->miscPort, 0x3C2);
    Port8Bit_init(&self->crtcIndexPort, 0x3D4);
    Port8Bit_init(&self->crtcDataPort, 0x3D5);
    Port8Bit_init(&self->sequencerIndexPort, 0x3C4);
    Port8Bit_init(&self->sequencerDataPort, 0x3C5);
    Port8Bit_init(&self->graphicsControllerIndexPort, 0x3CE);
    Port8Bit_init(&self->graphicsControllerDataPort, 0x3CF);
    Port8Bit_init(&self->attributeControllerIndexPort, 0x3C0);
    Port8Bit_init(&self->attributeControllerReadPort, 0x3C1);
    Port8Bit_init(&self->attributeControllerWritePort, 0x3C0);
    Port8Bit_init(&self->attributeControllerResetPort, 0x3DA);

    Driver_Init(&self->driver);
    self->driver.driverData = self;


}
void VideoGraphicsArray_deActivate(VideoGraphicsArray* self){
    
}

void VideoGraphicsArray_writeRegisters(VideoGraphicsArray* self, uint8_t* registers){

    //miscellaneous output register
    Port8Bit_Write(&self->miscPort, *(registers++));

    // sequencer registers
    for (uint8_t i = 0; i < 5; i++)
    {
        Port8Bit_Write(&self->sequencerIndexPort, i);
        Port8Bit_Write(&self->sequencerDataPort, *(registers++));
    }

    // CRTC registers
    Port8Bit_Write(&self->crtcIndexPort, 0x03);
    Port8Bit_Write(&self->crtcDataPort, Port8Bit_Read(&self->crtcDataPort) | 0x80); // Unlock CRTC registers
    Port8Bit_Write(&self->crtcIndexPort, 0x11);
    Port8Bit_Write(&self->crtcDataPort, Port8Bit_Read(&self->crtcDataPort) & ~0x80); 

    registers[0x03] = registers[0x03] | 0x80; // Unlock CRTC registers
    registers[0x11] = registers[0x11] & ~0x80; // Unlock CRTC registers

    for (uint8_t i = 0; i < 25; i++)
    {
        Port8Bit_Write(&self->crtcIndexPort, i);
        Port8Bit_Write(&self->crtcDataPort, *(registers++));
    }

    // graphics controller registers
    for (uint8_t i = 0; i < 9; i++)
    {
        Port8Bit_Write(&self->graphicsControllerIndexPort, i);
        Port8Bit_Write(&self->graphicsControllerDataPort, *(registers++));
    }

    // attribute controller registers
    for (uint8_t i = 0; i < 21; i++)
    {
        Port8Bit_Read(&self->attributeControllerResetPort); // Reset flip-flop
        Port8Bit_Write(&self->attributeControllerIndexPort, i);
        Port8Bit_Write(&self->attributeControllerWritePort, *(registers++));
    }
    
    Port8Bit_Read(&self->attributeControllerResetPort);
    Port8Bit_Write(&self->attributeControllerIndexPort, 0x20); // Enable video output

}

void VideoGraphicsArray_readRegisters(VideoGraphicsArray* self, uint8_t* registers){

}

bool VideoGraphicsArray_supportsMode(VideoGraphicsArray* self, uint32_t width, uint32_t height, uint32_t colorDepth){
    return width == 320 && height == 200 && colorDepth == 8;
}

bool VideoGraphicsArray_setMode(VideoGraphicsArray* self, uint32_t width, uint32_t height, uint32_t colorDepth){

    if (VideoGraphicsArray_supportsMode(self, width, height, colorDepth) != true){
        return false;
    }

    unsigned char g_320x200x256[] =
    {
        /* MISC */
        0x63,

        /* SEQ */
        0x03, 0x01, 0x0F, 0x00, 0x0E,

        /* CRTC */
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
        0x00, 0x41, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x9C, 0x0E, 0x8F, 0x28, 0x48, 0x96, 0xB9, 0xA3,
        0xFF,

        /* GC */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
        0xFF,

        /* AC */
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x41, 0x00, 0x0F, 0x00, 0x00
    };

    VideoGraphicsArray_writeRegisters(self, g_320x200x256);
    return true;

}

uint8_t* VideoGraphicsArray_getFrameBufferSegment(VideoGraphicsArray* self){
    Port8Bit_Write(&self->graphicsControllerIndexPort, 0x06);
    uint8_t segmentNumber =( Port8Bit_Read(&self->graphicsControllerDataPort) >> 2) & 0x03;
    switch (segmentNumber)
    {
        case 0: return (uint8_t*)0x00000; 
        case 1: return (uint8_t*)0xA0000; 
        case 2: return (uint8_t*)0xB0000; 
        case 3: return (uint8_t*)0xB8000; 
    }
}

void PutPixel(VideoGraphicsArray* self, uint32_t x, uint32_t y, uint8_t colorIndex){
    uint8_t* pixelAddress = VideoGraphicsArray_getFrameBufferSegment(self) + (y * 320 + x);
    *pixelAddress = colorIndex; // this will set the pixel at (x, y) to the specified color index
}


void VideoGraphicsArray_putPixel(VideoGraphicsArray* self, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b){
    uint8_t colorIndex = VideoGraphicsArray_getColorIndex(self, r, g, b);
    PutPixel(self, x, y, colorIndex);
}

uint8_t VideoGraphicsArray_getColorIndex(VideoGraphicsArray* self, uint8_t r, uint8_t g, uint8_t b){

    if(r == 0x00 && g == 0x00 && b == 0xA8) return 0x01; // Blue

}

void FillRectangle(VideoGraphicsArray* self,  uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t r, uint8_t g, uint8_t b){

    for (int32_t Y=y; Y < y + height; Y++){
        for(int32_t X=x; X < x + width; X++){
            VideoGraphicsArray_putPixel(self, X, Y, r, g, b);
        }
    }

}
