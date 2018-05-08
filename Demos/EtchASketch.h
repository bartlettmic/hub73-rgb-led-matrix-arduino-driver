#pragma once
// #include "../Definitions.h"
extern int16_t matrix[HEIGHT][WIDTH];
extern MPU6050 MPU;
extern trimpots knobs;

namespace EtchASketch {

void init() {
  memset(matrix, 0, sizeof(matrix));
}

void draw() {
  MPU.read();
  if (abs(MPU.ay) > 8000) init();
  knobs.read();
  matrix[knobs.l >> 5][knobs.r >> 5] = 0b0000010000100001;
//  delay(100);
}

}


