#pragma once
#include "Definitions.h"
#include "Utility.h"

extern MPU6050 MPU;

namespace Fluid {

void draw(int16_t matrix[HEIGHT][WIDTH]) {
  MPU.read();

  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      if (matrix[y][x] & 32768) continue;
      if (!matrix[y][x]) continue;

      byte dx = x + MPU.raw[0];    if (dx < 0 || dx >= WIDTH) continue;
      byte dy = y + MPU.raw[1];    if (dy < 0 || dy >= HEIGHT) continue;

      if (matrix[dy][dx]) continue;

      matrix[dy][dx] = matrix[y][x] | 32768; // Flag moved pixel
      matrix[y][x] = 32768; // Flag and blank old pixel

    }
  }

  // Clear all flags for next frame;
  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      matrix[y][x] &= ~32768;
    }
  }

  delay(15);
}

void setup(int16_t matrix[HEIGHT][WIDTH]) {
  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      randomSeed(analogRead(0) * analogRead(1) * analogRead(2));
      if (random(5) < 1) {
        //        byte c = random(256);
        byte c = 64;
        matrix[y][x] = RGBsmash(rainbow(c - 170), rainbow(c - 85), rainbow(c));
      }
      else {
        matrix[y][x] = 0;
      }
    }
  }
}
}

