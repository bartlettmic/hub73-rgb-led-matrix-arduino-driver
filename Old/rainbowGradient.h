#ifndef RAINBOWGRADIENT_H
#define RAINBOWGRADIENT_H

#include "definitions.h"
#include "utility.h"

void rainbowDraw(int16_t matrix[32][32]) {
  for (byte y = 0; y < HEIGHT; y++) {

    int count = 0;
//    int16_t* row = matrix[y];
//
//    for (byte s = 0; count < WIDTH; s++) {
//      int current = s;
//      int prev = row[s];
//      do {
//        int next = (current + 1) % WIDTH;
//        int tmp = row[next];
//        row[next] = prev;
//        prev = tmp;
//        current = next;
//        count++;
//      } while (s != current);
//    }
  }
//  ticks += 50;
//  delay((32768 - abs(ticks)) / 1000);
}

void rainbowSetup(int16_t matrix[32][32]) {
  for (byte y = 0; y < HEIGHT; y++) {
    byte dy = y << 2;
    for (byte x = 0; x < WIDTH; x++) {
      byte c = ((x << 3)  + dy);
      // F RRRRR GGGGG BBBBB
      matrix[y][x] = RGBsmash(rainbow(c - 170), rainbow(c - 85), rainbow(c), 4);
    }
  }
//  ticks = 65534;
}

#endif
