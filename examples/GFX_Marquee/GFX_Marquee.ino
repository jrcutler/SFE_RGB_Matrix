/* Example sketch for SFE Serial LED Matrix alternative firmware
 *
 * Arduino   -> Matrix
 * SS   (10) -> CS
 * MOSI (11) -> MISO
 * SCK  (13) -> SCLK
 * VCC       -> VCC
 * GND       -> VCC
 */
#include <SPI.h>
#include <SFE_RGB_Matrix.h>
#include <Adafruit_GFX.h>
#include "GFX_Marquee.h"

GFX_Marquee<2> marquee;

#define RED   0xf800
#define GREEN 0x07e0
#define BLUE  0x001f
#define WHITE 0xffff

void setup()
{
  SPI.begin();
}

void loop()
{
  uint16_t colors[] = { RED, GREEN, BLUE, WHITE };
  for (uint8_t r = 0; r < 4; ++r)
  {
    marquee.setRotation(r);
    for (uint8_t i = 0; i < sizeof(colors)/sizeof(colors[0]); ++i)
    {
      uint16_t color = colors[i];
      // draw image
      marquee.clear();
      marquee.drawChar(0, 0, (unsigned char)('0' + r), color, 0, 1);
      marquee.drawPixel(marquee.width()-1, marquee.height()-1, color);
      // display (single matrix)
      marquee.display();
      delay(1000);
    }
  }
}
