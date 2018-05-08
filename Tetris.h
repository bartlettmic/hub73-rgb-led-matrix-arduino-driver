#pragma once
#include "Definitions.h"
#include "Utility.h"
extern int16_t matrix[HEIGHT][WIDTH];
extern uint16_t ticks;
extern MPU6050 MPU;
extern trimpots knobs;

namespace Tetris {

void init() {
  memset(matrix, 0, sizeof(matrix));
}

void draw() {
//  knobs.read();  
//  byte y = knobs.l >> 5;
//  byte x = knobs.r >> 5;

  MPU.read();
  byte x = constrain(MPU.ax / 200, -WIDTH/2, WIDTH/2-1) + WIDTH/2;
  byte y = constrain(MPU.ay / 200, -HEIGHT/2, HEIGHT/2-1) + HEIGHT/2;
  
  init();
  circle(WIDTH/2, HEIGHT/2, sqrt(sq(x-WIDTH/2)+sq(y-HEIGHT/2)), 0b10000100001);
  line(x, y, WIDTH/2, HEIGHT/2, rainbowcycle(ticks));
//  line(x, y, WIDTH/2, HEIGHT/2, RGBsmash(255, 64, 0));
//  line(x, y, WIDTH/2, HEIGHT/2, 0b0000010000100000);
//  line(WIDTH-x-1, y, WIDTH/2-1, HEIGHT/2, 0b0000010000100001);
//  line(x, HEIGHT-y-1, WIDTH/2, HEIGHT/2-1, 0b0000010000100001);
//  line(WIDTH-x-1, HEIGHT-y-1, WIDTH/2-1, HEIGHT/2-1, 0b0000010000100001);
  delay(25);  
}

}


