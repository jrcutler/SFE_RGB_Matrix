#ifndef GFX_MATRIX_H
#define GFX_MATRIX_H

#include <SFE_RGB_Matrix.h>
#include <Adafruit_GFX.h>

class GFX_Matrix: public SFE_RGB_Matrix, public Adafruit_GFX
{
public:
  GFX_Matrix(uint8_t cs=SS);
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void invertDisplay();
};

#endif /* GFX_MATRIX_H */
