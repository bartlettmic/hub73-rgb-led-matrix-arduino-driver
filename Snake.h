#pragma once
#include "Definitions.h"
#include "Utility.h"

extern int16_t matrix[HEIGHT][WIDTH];
extern MPU6050 MPU;

namespace Snake {

void draw(int16_t matrix[HEIGHT][WIDTH]) {
  MPU.read();

}

void setup(int16_t matrix[HEIGHT][WIDTH]) {

}

}


