/* SparkFun Electronics RGB Matrix Serial Backpack alternative firmware */
#include "types.h"
#include "pins.h"


/*
 * Globals
 */


display_buffer_t display_buffer[2] = { 0, };
display_buffer_t *display_active = &display_buffer[0];
display_buffer_t *display_next = &display_buffer[1];

volatile rx_buffer_t rx = { 0, };

byte ticks = 0;


/*
 * Convenience macros
 */


#define sbi(reg, bit) ((reg) |= _BV(bit))
#define cbi(reg, bit) ((reg) &= (~_BV(bit)))

#define COLOR(c) (c)


/*
 * Interrupt vectors
 */


// SPI interrupt handler
ISR(SPI_STC_vect)
{
  byte data = SPDR;

  // save incoming data
  rx.raw[rx.index] = data;

  // extract color information from current byte
  switch (rx.subpixel)
  {
    case 0:
    {
      display_next->red[rx.pixel] = COLOR(data >> 4);
      display_next->green[rx.pixel] = COLOR(data & 0xf);
      ++rx.subpixel;
    }
    break;
    case 1:
    {
      display_next->blue[rx.pixel++] = COLOR(data >> 4);
      display_next->red[rx.pixel] = COLOR(data & 0xf);
      ++rx.subpixel;
    }
    break;
    case 2:
    {
      display_next->green[rx.pixel] = COLOR(data >> 4);
      display_next->blue[rx.pixel++] = COLOR(data & 0xf);
      rx.subpixel = 0;
    }
    break;
  }

  // increment index in circular buffer
  ++rx.index;
  if (rx.index == sizeof(rx.raw))
  {
    rx.index = 0;
    rx.pixel = 0;
    rx.subpixel = 0;
  }

  // send previous buffer data
  SPDR = rx.raw[rx.index];
}


/*
 * Support functions
 */


void refresh_row_color(const byte *color)
{
  const byte tick = ticks;
  byte index = 0;
  while (index < 8)
  {
    // lower clock
    cbi(PORT_SR, PIN_SR_SCK);
    // output color value for this tick
    if (color[index++] > tick)
    {
      sbi(PORT_SR, PIN_SR_DATA);
    }
    else
    {
      cbi(PORT_SR, PIN_SR_DATA);
    }
    // clock in data
    sbi(PORT_SR, PIN_SR_SCK);
  }
}


void refresh_display(const display_buffer_t *p)
{
  // output rows (reversed)
  const byte *red = p->red + 64;
  const byte *green = p->green + 64;
  const byte *blue = p->blue + 64;
  for (byte row = 0; row < 8; ++row)
  {
    // prepare to latch in new data
    cbi(PORT_SR, PIN_SR_LATCH);

    // output colors (in RBG order, unlike schematic)
    red -= 8;
    refresh_row_color(red);
    blue -= 8;
    refresh_row_color(blue);
    green -= 8;
    refresh_row_color(green);

    // disable shift register outputs
    sbi(PORT_SR, PIN_SR_EN);
    // latch in new data
    sbi(PORT_SR, PIN_SR_LATCH);
    // enable row
    PORT_ROW = _BV(row);
    // enable shift register outputs
    cbi(PORT_SR, PIN_SR_EN);
  }
}


void handle_chip_select(void)
{
  static byte cs, cs_prev = PIN_SPI & _BV(PIN_CS);

  // get current chip select state
  cs = PIN_SPI & _BV(PIN_CS);

  // check for chip select low to high transition
  if (!cs_prev && cs)
  {
    display_buffer_t *tmp;
    // swap display buffers and reset receive state (atomically)
    cli();
    tmp = display_active;
    display_active = display_next;
    display_next = tmp;
    rx.index = 0;
    rx.pixel = 0;
    rx.subpixel = 0;
    sei();
  }

  // save current state
  cs_prev = cs;
}


/*
 * Core functions
 */


void setup()
{
  // setup row select
  DDR_ROW  = 0xff;
  PORT_ROW = 0;

  // setup shift register output
  DDR_SR  = 0xff;
  PORT_SR = _BV(PIN_SR_CLEAR);

  // setup SPI slave
  DDR_SPI  = _BV(PIN_MISO);

  // enable SPI & SPI interrupts
  SPCR |= _BV(SPE) | _BV(SPIE);
}


void loop()
{
  while (true)
  {
    handle_chip_select();

    refresh_display(display_active);

    // update ticks
    ++ticks;
    ticks &= 0xf;
  }
}
