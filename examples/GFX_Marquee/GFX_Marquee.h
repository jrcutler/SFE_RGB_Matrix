#ifndef GFX_MARQUEE_H
#define GFX_MARQUEE_H

#include <SFE_RGB_Matrix.h>
#include <Adafruit_GFX.h>

template<uint8_t count>
class GFX_Marquee: public Adafruit_GFX
{
public:
  GFX_Marquee(uint8_t cs=SS)
  {
    panel[0] = SFE_RGB_Matrix(cs);
    constructor(8*count, 8);
  }

  void clear()
  {
    uint8_t m = count;
    while (m--)
    {
      panel[m].clear();
    }
  }

  void display()
  {
    panel[0].start();
    uint8_t m = count;
    while (m--)
    {
      panel[m].send();
    }
    panel[0].stop();
  }

  void drawPixel(int16_t x, int16_t y, uint16_t color)
  {
    uint8_t rotation;

    if ((x < 0) || (x >= width())) return;
    if ((y < 0) || (y >= height())) return;

    uint8_t m;

    // check rotation and rewrite coordinates
    rotation = getRotation();
    switch (rotation)
    {
    case 0:
      m = x >> 3;
      x &= 0x7;
      break;
    case 1:
      y = height() - y - 1;
      swap(x, y);
      m = x >> 3;
      x &= 0x7;
      break;
    case 2:
      x = width() - x - 1;
      y = height() - y - 1;
      m = x >> 3;
      x &= 0x7;
      break;
    case 3:
      x = width() - x - 1;
      swap(x, y);
      m = x >> 3;
      x &= 0x7;
      break;
    }

    panel[m].setPixel(x, y, color);
  }

private:
  SFE_RGB_Matrix panel[count];
};

#endif /* GFX_MARQUEE_H */
