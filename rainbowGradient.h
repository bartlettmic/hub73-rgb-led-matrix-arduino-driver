#ifndef RAINBOWGRADIENT_H
#define RAINBOWGRADIENT_H

#include "definitions.h"
#include "utility.h"


/*
  #define rainbowpixel(x)\
  c = c1 + (x << 2);\
  matrix[y][x][0] = rainbow(c - 170);\
  matrix[y][x][1] = rainbow(c - 85);\
  matrix[y][x][2] = rainbow(c);\

  #define rainbowrow(y)\
  c1 = (y << 2) + ticks;\
  rainbowpixel(0) rainbowpixel(1) rainbowpixel(2) rainbowpixel(3) rainbowpixel(4) rainbowpixel(5) rainbowpixel(6) rainbowpixel(7)\
  rainbowpixel(8) rainbowpixel(9) rainbowpixel(10) rainbowpixel(11) rainbowpixel(12) rainbowpixel(13) rainbowpixel(14) rainbowpixel(15)\
  rainbowpixel(16) rainbowpixel(17) rainbowpixel(18) rainbowpixel(19) rainbowpixel(20) rainbowpixel(21) rainbowpixel(22) rainbowpixel(23)\
  rainbowpixel(24) rainbowpixel(25) rainbowpixel(26) rainbowpixel(27) rainbowpixel(28) rainbowpixel(29) rainbowpixel(30) rainbowpixel(31)
*/

void rainbowDraw() {
  for (byte y = 0; y < HEIGHT; y++) {

    int count = 0;
    int16_t* row = matrix[y];

    for (byte s = 0; count < WIDTH; s++) {
      int current = s;
      int prev = row[s];
      do {
        int next = (current + 1) % WIDTH;
        int tmp = row[next];
        row[next] = prev;
        prev = tmp;
        current = next;
        count++;
      } while (s != current);
    }
  }
  ticks += 50;
  delay((32768 - abs(ticks)) / 1000);
}

void rainbowSetup() {
  for (byte y = 0; y < HEIGHT; y++) {
    byte dy = y << 2;
    for (byte x = 0; x < WIDTH; x++) {
      byte c = ((x << 3)  + dy);
      // F RRRRR GGGGG BBBBB
      matrix[y][x] = RGBsmash(rainbow(c - 170), rainbow(c - 85), rainbow(c), 4);
    }
  }
  ticks = 65534;
}

#endif
