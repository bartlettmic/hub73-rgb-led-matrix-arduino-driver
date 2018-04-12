#include "draws.h"
#include "matrix.h"
//#include "accelerometer.h"

int8_t acceleration[2] = {0};
int16_t ticks = 65535;
volatile byte mode = 0;
void (*draw)();

int16_t matrix[HEIGHT][WIDTH] = { 0 };

void setup() {
      Serial.begin(38400);

  const uint8_t PINS[] = {  //  76543210
    2,  3,  4,  5,  6,  7,  //D BGRBGR✕✕ Row 0-1
    14, 15, 16, 17, 18, 19, //C ✕✕BGRBGR Row 2-3
    8,  9,  10, 11,         //B ✕✕✕✕LABC
    20                      //⌚
  };
  for (auto pin : PINS) pinMode(pin, OUTPUT);

  matrixSetup();
//  accSetup();
  drawSetups[mode]();
  draw = drawModes[mode];  
}

void loop() {
  draw();
//  accRead(acceleration);
  ticks--;
}


