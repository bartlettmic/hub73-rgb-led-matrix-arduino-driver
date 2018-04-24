#include "Matrix.h"
#include "Accelerometer.h"
#include "Demos.h"
#include <EEPROM.h>

volatile byte mode = 0;
volatile byte buttonTicks = 1;

uint16_t ticks = 65535;


void BUTTON_ISR(void) {
  delay(1);
  if (!++buttonTicks) {
    ++buttonTicks;
    ++mode %= sizeof(demoLoops) / sizeof(*demoLoops);    
    draw = demoLoops[mode];
    demoSetups[mode]();
  }
}

void setup() {
  //  Serial.begin(38400);

  const uint8_t PINS[] = {  //  76543210
    2,  3,  4,  5,  6,  7,  //D BGRBGR✕✕ Row 0-1
    14, 15, 16, 17, 18, 19, //C ✕✕BGRBGR Row 2-3
    8,  9,  10, 11,         //B ✕✕✕✕LABC
    20, 13                  //⌚, OE
  };
  for (auto pin : PINS) pinMode(pin, OUTPUT);

  pinMode(12, INPUT);   // Mode change button
  attachInterrupt(digitalPinToInterrupt(12), BUTTON_ISR, LOW);


  matrixSetup();

  MPU.setup(22, 23);

  //  mode = EEPROM.read(0);
  //  EEPROM.write(0, ((mode+1) % (sizeof(drawModes)/sizeof(*drawModes))));

  demoSetups[mode]();
  draw = demoLoops[mode];
}

void loop() {
  draw();
  ticks++;
}
