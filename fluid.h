#ifndef FLUID_H
#define FLUID_H
#include "definitions.h"
#include "utility.h"

void fluidDraw() {
  
}

void fluidSetup() {
  for (byte y=0; y < HEIGHT; y++) {
    for (byte x=0; x < WIDTH; x++) {
      randomSeed(analogRead(0) * analogRead(1) * analogRead(2));
      if (random(4)<1) {
        byte c = random(256);
        //matrix[y][x] = (random(256) << 7) | (random(256) << 2) | (random(256) >> 3);
        matrix[y][x] = 32768 | RGBsmash(rainbow(c - 170), rainbow(c - 85), rainbow(c));
      }
      else {
        matrix[y][x] = 0;
      }
    }
  }
}

#endif

