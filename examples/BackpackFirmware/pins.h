#ifndef PINS_H
#define PINS_H

#include <avr/io.h>
#include "types.h"

/*
 * SPI port/pin definitions
 */

#define PORT_SPI PORTB
#define PIN_SPI  PINB
#define DDR_SPI  DDRB

const byte PIN_CS   = PINB2;
const byte PIN_MOSI = PINB3;
const byte PIN_MISO = PINB4;
const byte PIN_SCK  = PINB5;

/*
 * Shift register port/pin definitions
 */

#define PORT_SR PORTC
#define DDR_SR  DDRC

const byte PIN_SR_SCK   = PINC0;
const byte PIN_SR_DATA  = PINC1;
const byte PIN_SR_LATCH = PINC2;
const byte PIN_SR_CLEAR = PINC3;
const byte PIN_SR_EN    = PINC4;

/*
 * Row select port definitions
 */

#define PORT_ROW PORTD
#define DDR_ROW  DDRD

#endif /* PINS_H */
