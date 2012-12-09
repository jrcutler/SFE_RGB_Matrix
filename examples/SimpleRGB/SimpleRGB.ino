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

SFE_RGB_Matrix matrix;

#define RED   0xf800
#define GREEN 0x07e0
#define BLUE  0x001f

void setup()
{
  SPI.begin();
}

void loop()
{
  uint16_t colors[] = { RED, GREEN, BLUE };
  for (uint8_t i = 0; i < sizeof(colors)/sizeof(colors[0]); ++i)
  {
    // fill with color
    for (uint8_t n = 0; n < 64; ++n)
    {
      matrix.setPixel(n, colors[i]);
    }
    // display (single matrix)
    matrix.display();
    delay(1000);
  }
}
