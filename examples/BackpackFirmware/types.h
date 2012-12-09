#ifndef TYPES_H
#define TYPES_H

#include <Arduino.h>

typedef struct
{
  byte red[64];
  byte green[64];
  byte blue[64];
} display_buffer_t;

typedef struct
{
  byte raw[96];
  byte index;
  byte subpixel;
  byte pixel;
} rx_buffer_t;

#endif /* _BUFFER_H */
