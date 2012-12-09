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

// change this to match the number of chained matrices
#define MATRIX_COUNT 2

SFE_RGB_Matrix matrix[MATRIX_COUNT];

#define RED   0xf800
#define GREEN 0x07e0
#define BLUE  0x001f

void fill_all(uint16_t color)
{
  for (uint8_t m = 0; m < MATRIX_COUNT; ++m)
  {
    for (uint8_t n = 0; n < 64; ++n)
    {
      matrix[m].setPixel(n, color);
    }
  }
}

void display_all()
{
  matrix[0].start();
  for (uint8_t m = 0; m < MATRIX_COUNT; ++m)
  {
    matrix[m].send();
  }
  matrix[0].stop();
}

void setup()
{
  SPI.begin();
}

void loop()
{
  uint16_t colors[] = { RED, GREEN, BLUE };
  for (uint8_t i = 0; i < sizeof(colors)/sizeof(colors[0]); ++i)
  {
    fill_all(colors[i]);
    display_all();
    delay(1000);
  }
}
