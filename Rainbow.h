#pragma once
#include "Definitions.h"
#include "Utility.h"

extern int16_t matrix[HEIGHT][WIDTH];

namespace Rainbow {

void draw() {
  for (byte y = 0; y < HEIGHT; y++) {

    byte count = 0;

    for (byte x = 0; count < WIDTH; x++) {
      byte current = x;
      int16_t prev = matrix[y][x];
      do {
        byte next = (current + 1) % WIDTH;
        int16_t tmp = matrix[y][next];
        matrix[y][next] = prev;
        prev = tmp;
        current = next;
        count++;
      } while (x != current);
    }
  }

  //  ticks += 51;
  //  delay((32768 - abs(ticks)) / 1000);
  delay(30);
}

void setup() {
  memset(matrix, 0, sizeof(matrix));
  for (byte y = 0; y < HEIGHT; y++) {
    byte dy = y << 2;
    for (byte x = 0; x < WIDTH; x++) {
      byte c = ((x << 3)  + dy);
      // F RRRRR GGGGG BBBBB
      matrix[y][x] = RGBsmash(rainbow(c - 170), rainbow(c - 85), rainbow(c));
      //      matrix[y][x] = RGBsmash(127, 32, 0);
    }
  }
}
}

