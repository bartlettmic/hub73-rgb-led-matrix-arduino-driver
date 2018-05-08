#pragma once
// #include "Definitions.h"
// #include "Utility.h"
extern int16_t matrix[HEIGHT][WIDTH];
extern MPU6050 MPU;

namespace Snake {                                                       //TO-DO Fix this, make snake loop, stop on body collision

int8_t dX = 0, dY = 0, X = WIDTH / 2, Y = HEIGHT / 2, fruit[2];
int16_t length = 5;

void init() {
  do {
    srand(micros());
    fruit[0] = random(WIDTH);
    fruit[1] = random(HEIGHT);
  } while (matrix[fruit[1]][fruit[0]]);
}

void draw() {
  MPU.read();
  dX = MPU.ax > 1000 ? 1 : MPU.ax < -1000 ? -1 : 0;
  dY = MPU.ay > 1000 ? 1 : MPU.ay < -1000 ? -1 : 0;
  bool _4way = abs(MPU.ax) > abs(MPU.ay);
  dX = _4way ? dX : 0;
  dY = _4way ? 0 : dY;
  byte x = X + dX;
  byte y = Y + dY;
  if (x < 0 || x >= WIDTH)  return;
  if (y < 0 || x >= HEIGHT) return;
  X = x;
  Y = y;

  if (!matrix[Y][X]) {      //Spot is empty
    for ( int16_t* p = matrix[0]; p < &matrix[HEIGHT - 1][WIDTH - 1]; ++p ) if (*p > 0) (--((*p) >>= 5)) <<=5;
    matrix[Y][X] = length << 5;  //Place new snake head
    matrix[fruit[1]][fruit[0]] = 0b0111110000000000;
  }

  else if (X == fruit[0] && Y == fruit[1]) {
    matrix[Y][X] = length << 5;  //Place new snake head
    length++; //Increase snake length
    init();  //Place new fruit
    return;   //Don't blank any snake parts
  }

//  else {  // Bad Ouroboros!
//    fillSpiral(matrix, 0b0000010000000000, 10, true);
//    fillSpiral(matrix, 0, 10, false);
//    X = WIDTH / 2;
//    Y = HEIGHT / 2;
//    length=1;
//    init();
//  }
  
  delay(100);
}

}


