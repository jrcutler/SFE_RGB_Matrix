#ifndef SFE_RGB_MATRIX_H
#define SFE_RGB_MATRIX_H

#include <Arduino.h>

class SFE_RGB_Matrix
{
public:
    SFE_RGB_Matrix(uint8_t cs=SS);

    // display a single matrix
    void display();

    // start transfer
    void start();
    // send pixel data
    void send();
    // stop transfer and refresh display
    void stop();

    void clear();
    void setPixel(uint8_t x, uint8_t y, uint16_t color);
    void setPixel(uint8_t n, uint16_t color);

    static inline uint16_t rgb12_color(byte r, byte g, byte b)
    {
        uint16_t color = 0;
        color |= (r & 0xf) << (11 + 1);
        color |= (g & 0xf) << (5 + 2);
        color |= (b & 0xf) << (0 + 1);
        return color;
    }

    static inline uint16_t rgb24_color(byte r, byte g, byte b)
    {
        uint16_t color = 0;
        color |= (r >> 3) << 11;
        color |= (g >> 2) << 5;
        color |= (b >> 3) << 0;
        return color;
    }

protected:
    byte raw_buffer[96];

private:
    uint8_t _cs_pin;
};

#endif /* SFE_RGB_MATRIX_H */