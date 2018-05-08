#include "Matrix.h"
#include "Accelerometer.h" //TO-DO Combine these into "Inputs.h"
#include "Trimpots.h"
#include "Demos.h"
//#include <EEPROM.h>

byte mode = 6;
//volatile byte buttonTicks = 1;

uint16_t ticks = 65535;

void setup() {
//    Serial.begin(38400);

  const uint8_t PINS[] = {  //  76543210
    2,  3,  4,  5,  6,  7,  //D BGRBGR✕✕ Row 0-1
    14, 15, 16, 17, 18, 19, //C ✕✕BGRBGR Row 2-3
    8,  9,  10, 11, 12, 13  //B ✕✕~⌚LABC
  };
  for (auto pin : PINS) pinMode(pin, OUTPUT);

  pinMode(20, INPUT);
  pinMode(21, INPUT);

  //  pinMode(12, INPUT);   // Mode change button
  //  attachInterrupt(digitalPinToInterrupt(12), BUTTON_ISR, LOW);


  matrixSetup();

  MPU.setup(22, 23);

//  mode = EEPROM.read(0);
//  EEPROM.write(h0, ((mode + 1) % (sizeof(demoLoops) / sizeof(*demoLoops))));

  demoSetups[mode]();
  draw = demoLoops[mode];
}

void loop() {
  draw();
  ticks++;
}
