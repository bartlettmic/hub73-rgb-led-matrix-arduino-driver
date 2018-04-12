#pragma once
#ifndef UTILITY_H
#define UTILITY_H

uint16_t RGBsmash(byte R, byte G, byte B) {
  return ((R & 248) << 7) | ((G & 248) << 2) | (B >> 3);
}

uint16_t RGBsmash(byte R, byte G, byte B, byte dim) {
  return (((R/dim) & 248) << 7) | (((G/dim) & 248) << 2) | ((B/dim) >> 3);
}

byte rainbow(byte i) {
  if (i > 170) return 255 - i * 3;
  if (i > 85)  return i * 3;
  return 0;
}

#endif
