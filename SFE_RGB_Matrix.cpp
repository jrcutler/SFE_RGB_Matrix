#include <SFE_RGB_Matrix.h>
#include <SPI.h>


/* time to delay after every byte */
#define DELAY_PER_BYTE 16
/* time to delay after every stop */
#define DELAY_PER_STOP 32


#define MASK_5 ((1 << 5) - 1)
#define MASK_6 ((1 << 6) - 1)


SFE_RGB_Matrix::SFE_RGB_Matrix(uint8_t cs)
{
    _cs_pin = cs;
    pinMode(_cs_pin, OUTPUT);
}


void SFE_RGB_Matrix::clear()
{
    for (uint8_t i = 0; i < sizeof(raw_buffer); ++i)
    {
        raw_buffer[i] = 0x00;
    }
}


void SFE_RGB_Matrix::setPixel(uint8_t n, uint16_t color)
{
    if (n >= 64)
    {
        return;
    }

    // separate color components
    uint8_t red, green, blue;
    red = (color >> 11) & MASK_5;
    green = (color >> 5) & MASK_6;
    blue = color & MASK_5;

    // map colors to 12-bit (4/4/4) space
    red >>= 1;
    green >>= 2;
    blue >>= 1;

    // set pixel in raw buffer
    uint8_t offset = n + (n >> 1);
    if (n & 1)
    {
        // odd pixel
        raw_buffer[offset] &= 0xf0;
        raw_buffer[offset] |= red;
        ++offset;
        raw_buffer[offset]  = green << 4 | blue;

    }
    else
    {
        // even pixel
        raw_buffer[offset]  = red << 4 | green;
        ++offset;
        raw_buffer[offset] &= 0x0f;
        raw_buffer[offset] |= blue << 4;
    }
}


void SFE_RGB_Matrix::setPixel(uint8_t x, uint8_t y, uint16_t color)
{
    setPixel(y*8 + x, color);
}


void SFE_RGB_Matrix::start()
{
    digitalWrite(_cs_pin, LOW);
}


void SFE_RGB_Matrix::send()
{
    // send raw buffer
    for (uint8_t i = 0; i < sizeof(raw_buffer); ++i)
    {
        SPI.transfer(raw_buffer[i]);
        delayMicroseconds(DELAY_PER_BYTE);
    }
}


void SFE_RGB_Matrix::stop()
{
    digitalWrite(_cs_pin, HIGH);
    delayMicroseconds(DELAY_PER_STOP);
}


void SFE_RGB_Matrix::display()
{
    start();
    send();
    stop();
}
