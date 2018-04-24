#pragma once
#include "Definitions.h"
#include "Utility.h"

#define PONG_MPU_SENSITIVITY 200
#define PADDLE_SIZE 12
#define HALF_PADDLE_SIZE PADDLE_SIZE / 2

extern int16_t matrix[HEIGHT][WIDTH];
extern MPU6050 MPU;

const PROGMEM int16_t p1portal = RGBsmash(255, 64, 0);
const PROGMEM int16_t p2portal = RGBsmash(0, 64, 255);
const PROGMEM int16_t ballcolor = RGBsmash(64, 64, 64);

namespace Pong {

struct Dot {
  int16_t X, Y;
  uint8_t x, y;
  int16_t dx, dy;
  bool left = true;
  bool pong = false;

  void reset(void) {
    x = byte(WIDTH / 2);
    y = byte(HEIGHT / 2);
    X = (WIDTH / 2) * SUBPIXEL_PRECISION;
    Y = (HEIGHT / 2) * SUBPIXEL_PRECISION;
    dx = left ? -50 : 50;
    dy = -3;
  }

  bool update(void) {

    byte _x = (X + dx) / SUBPIXEL_PRECISION;
    left = _x < 1;
    bool changedX = left || _x > WIDTH - 2;
    if (changedX) {
      x = !pong ? left ? WIDTH - 1 : 0 : (X + (dx *= -1))/SUBPIXEL_PRECISION;
      X = x * SUBPIXEL_PRECISION;
    }
    else x = _x;
    X += dx;

   byte _y = (Y + dy) / SUBPIXEL_PRECISION;
    if (_y < 0 || _y >= HEIGHT) y = (Y + (dy *= -1)) / SUBPIXEL_PRECISION;
    else y = _y;
    Y += dy;

    if (changedX) {
      srand(analogRead(0));
      dx += dx > 0 ? 5 : -5;
      dy += (30 - random(60)) * (dy > 0 ? 1 : -1);
    }

    return changedX;
  }

} ball;

void draw() {

  matrix[ball.y][ball.x] = 0; // Clear last pixel position

  if (ball.update()) {
    if (ball.left) {
      if (!matrix[ball.y][0]) {
        delay(1000);
        ball.reset();
      }
    }
    else {
      if (!matrix[ball.y][WIDTH - 1]) {
        delay(1000);
        ball.reset();
      }
    }
  }

  matrix[ball.y][ball.x] = ballcolor;

  MPU.read();
  byte cy = constrain(max(MPU.raw[1], 0) / PONG_MPU_SENSITIVITY, HALF_PADDLE_SIZE, HEIGHT - HALF_PADDLE_SIZE);

  byte p = 0;
  for ( ; p < cy - HALF_PADDLE_SIZE; p++) matrix[p][0] = 0, matrix[p][WIDTH - 1] = 0;
  for ( ; p < cy + HALF_PADDLE_SIZE; p++) matrix[p][0] = ball.pong ? ballcolor : p1portal, matrix[p][WIDTH - 1] = ball.pong ? ballcolor : p2portal;
  for ( ; p < HEIGHT; p++) matrix[p][0] = 0, matrix[p][WIDTH - 1] = 0;
}

void setup() {
  //  Serial.begin(38400);
  memset(matrix, 0, sizeof(matrix));  
  ball.reset();
  ball.pong = !ball.pong;
}

}


