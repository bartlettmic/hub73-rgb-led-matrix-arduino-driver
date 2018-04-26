#pragma once
#include "Definitions.h"
#include "Utility.h"

#define POPULATION 256 // Must be a power of 2 or things will break :^)
#define TERMINAL_VELOCITY 3000

extern int16_t matrix[HEIGHT][WIDTH];
extern MPU6050 MPU;
extern uint16_t ticks;

namespace Fluid {



struct Dot {
  int16_t x, y, dx, dy;

  Dot(byte x, byte y) {
    this->x = x << SUBPIXEL_SHIFTS;
    this->y = y << SUBPIXEL_SHIFTS;
    //    dx = dy = 0;
    //    srand(analogRead(0));
    //    dx = 50 - random(100);
    //    dy = 50 - random(100);
  }

  Dot() {}

  byte update(void) {
    byte X = constrain(x >> SUBPIXEL_SHIFTS, 0, WIDTH  - 1); // Truncate current subpixel coordinates
    byte Y = constrain(y >> SUBPIXEL_SHIFTS, 0, HEIGHT - 1);
    byte _x = (x + dx) >> SUBPIXEL_SHIFTS;
    byte _y = (y + dy) >> SUBPIXEL_SHIFTS;
    if (_x < 0 || _x >= WIDTH)  _x = (x + (dx /= -2)) >> SUBPIXEL_SHIFTS;
    if (_y < 0 || _y >= HEIGHT) _y = (y + (dy /= -2)) >> SUBPIXEL_SHIFTS;

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

void draw() {
  MPU.read();

  int16_t ax = MPU.raw[0] >> 1;
  int16_t ay = MPU.raw[1] >> 1;

  if (Serial.available()) Serial.println(ay);

  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      if (!matrix[y][x])        continue; // Empty
      if (matrix[y][x] & 32768) continue; // Already updated for this frame

      byte id = matrix[y][x] & (POPULATION - 1);




      dots[id].dx += ax;  // Physics 'n' such
      if (abs(dots[id].dx) > TERMINAL_VELOCITY) dots[id].dx = dots[id].dx > 0 ? TERMINAL_VELOCITY : -TERMINAL_VELOCITY;
      dots[id].dy += ay;
      if (abs(dots[id].dy) > TERMINAL_VELOCITY) dots[id].dy = dots[id].dy > 0 ? TERMINAL_VELOCITY : -TERMINAL_VELOCITY;

      byte _id = dots[id].update();
      if (_id > 0) {
        //        dots[id].dx = dots[_id].dx = (dots[_id].dx + dots[id].dx) /2;  // Conservation of momentum
        //                dots[id].dy = dots[_id].dy = (dots[_id].dy + dots[id].dy) /2;
        //                dots[id].dx  = dots[id].dy = 0;
        //        dots[id].dx  *= -1;
        //        dots[_id].dx *= -1;
        //        dots[id].dy  *= -1;
        //        dots[_id].dy *= -1;
        dots[id].dx  /= -2;
        dots[_id].dx /= -2;
        dots[id].dy  /= -2;
        dots[_id].dy /= -2;
      }

    }
  }

  // Clear all flags for next frame;
  for ( int16_t* p = matrix[0]; p < &matrix[HEIGHT - 1][WIDTH - 1]; ++p ) *p &= ~32768;
  delay(20);
}

void setup() {
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

}


