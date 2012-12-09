#include "GFX_Matrix.h"

GFX_Matrix::GFX_Matrix(uint8_t cs):
  SFE_RGB_Matrix(cs)
{
  constructor(8, 8);
}


void GFX_Matrix::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  uint8_t rotation;

  if ((x < 0) || (x >= width())) return;
  if ((y < 0) || (y >= height())) return;

  // check rotation and rewrite coordinates
  rotation = getRotation();
  switch (rotation)
  {
  case 0:
    break;
  case 1:
    y = height() - y - 1;
    swap(x, y);
    break;
  case 2:
    x = width() - x - 1;
    y = height() - y - 1;
    break;
  case 3:
    x = width() - x - 1;
    swap(x, y);
    break;
  }

  setPixel(x, y, color);
}


void GFX_Matrix::invertDisplay()
{
  for (uint8_t i = 0; i < sizeof(raw_buffer); ++i)
  {
    raw_buffer[i] = ~raw_buffer[i];
  }
}
