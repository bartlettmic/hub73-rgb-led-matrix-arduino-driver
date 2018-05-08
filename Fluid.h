#pragma once
#include "Definitions.h"
#include "Utility.h"

#define POPULATION 256 // Must be a power of 2 or things will break :^)
#define TERMINAL_VELOCITY 32
#define SUBPIXEL_SHIFT 2      // log_2(128/32) = 2 i.e. 1 << 2 == 4

extern int16_t matrix[HEIGHT][WIDTH];
extern MPU6050 MPU;
extern uint16_t ticks;

namespace Fluid {



struct Dot {                    //TO-DO 8bit subpixels
  int8_t dx, dy;
  uint8_t x, y;

  Dot(byte x, byte y) {
    this->x = x << SUBPIXEL_SHIFT;
    this->y = y << SUBPIXEL_SHIFT;
    //    dx = dy = 0;
    //    srand(analogRead(0));
    //    dx = 50 - random(100);
    //    dy = 50 - random(100);
  }

  Dot() {}

  byte update(void) {
    byte X = constrain(x >> SUBPIXEL_SHIFT, 0, WIDTH  - 1); // Truncate current subpixel coordinates
    byte Y = constrain(y >> SUBPIXEL_SHIFT, 0, HEIGHT - 1);
    byte _x = (x + dx) >> SUBPIXEL_SHIFT;
    byte _y = (y + dy) >> SUBPIXEL_SHIFT;
    if (_x < 0 || _x >= WIDTH)  _x = (x + (dx /= -2)) >> SUBPIXEL_SHIFT;
    if (_y < 0 || _y >= HEIGHT) _y = (y + (dy /= -2)) >> SUBPIXEL_SHIFT;

    _x = constrain(_x, 0,  WIDTH - 1);
    _y = constrain(_y, 0, HEIGHT - 1);

    matrix[Y][X] |= 32768; // Flag particle as updated for this frame

    if (matrix[_y][_x]) {   // Collision! :^)
      return matrix[_y][_x] & (POPULATION - 1); // Identify the offending particle
    }                       // Don't update coordinates tho

    else {  // The spot is vacant, time to move
      matrix[_y][_x] = matrix[Y][X]; // Migrate
      matrix[Y][X] = 0;              // Open up old spot for other particles
      x += dx;
      y += dy; // Update subpixel coordinates since they're valid
    }

    return 0;
  }
};

Dot dots[POPULATION];

#define MAX_ACCEL 8

void init() {
  memset(matrix, 0, sizeof(matrix));
  byte x = 0, y = 0;
  uint16_t id = 1;

  for ( ; id < POPULATION; id++) {
    srand(analogRead(0));
    while (matrix[y][x]) {
      x = random(WIDTH);
      y = random(HEIGHT);
    }

    dots[id] = Dot(x, y);
    //    if (!(++x &= WIDTH - 1)) ++y;


    int16_t color = 80;
    color = RGBsmash(rainbow(color - 170), rainbow(color - 85), rainbow(color)) >> 1;
    color &= ~(POPULATION - 1);
    color |= id;              // Store ID in Frame Buffer with Steganography

    matrix[y][x] = color;
  }

}

void draw() {
  MPU.read();


  int16_t ax = constrain(MPU.ax >> 9, -MAX_ACCEL, MAX_ACCEL);
  int16_t ay = constrain(MPU.ay >> 9, -MAX_ACCEL, MAX_ACCEL);
  //  int16_t ax = MPU.ax / 750;
  //  int16_t ay = MPU.ay / 750;

  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      if (!matrix[y][x])        continue; // Empty
      if (matrix[y][x] & 32768) continue; // Already updated for this frame

      byte id = matrix[y][x] & (POPULATION - 1);

      byte _id = dots[id].update();
      if (_id > 0) {
        //        dots[id].dx = dots[_id].dx = (dots[_id].dx + dots[id].dx) /2;  // Conservation of momentum
        //                dots[id].dy = dots[_id].dy = (dots[_id].dy + dots[id].dy) /2;
        //                dots[id].dx  = dots[id].dy = 0;

        dots[id].dx  /= -2;
        //        dots[_id].dx *= -1;
        dots[id].dy  /= -2;
        //        dots[_id].dy *= -1;

        //        int8_t _dx = dots[id].dx;
        //        int8_t dir = sgn(_dx) * sgn(dots[id].dy);
        //        dots[id].dx = dir * dots[id].dy / -2;
        //        dots[id].dy = dir * _dx / -2;
      }

      dots[id].dx = max(min(int16_t(dots[id].dx) + ax, TERMINAL_VELOCITY), -TERMINAL_VELOCITY);
      dots[id].dy = max(min(int16_t(dots[id].dy) + ay, TERMINAL_VELOCITY), -TERMINAL_VELOCITY);
      //        dots[id].dx += ax;
      //        dots[id].dy += ay;

    }
  }

  // Clear all flags for next frame;
  for ( int16_t* p = matrix[0]; p < &matrix[HEIGHT - 1][WIDTH - 1]; ++p ) *p &= ~32768;
  delay(20);
}

}


