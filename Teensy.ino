#include "PWMISR_unroll.h"
#include "Accelerometer.h"
#include <EEPROM.h>

#define WIDTH 32
#define HEIGHT 32

volatile byte wait = 8;
volatile uint16_t Rmask = (wait << 7);
volatile uint16_t Gmask = (wait << 2);
volatile uint16_t Bmask = (wait >> 3);
volatile byte abc = 0;
volatile byte rows[4] = { 0, 8, 16, 24 };


volatile byte mode = 1;

int16_t matrix[HEIGHT][WIDTH] = { 0 };
int16_t ticks = 65535;
int16_t acceleration[2] = {0};

uint16_t RGBsmash(byte R, byte G, byte B) {
  return ((R & 248) << 7) | ((G & 248) << 2) | (B >> 3);
}

uint16_t RGBsmash(byte R, byte G, byte B, byte dim) {
  return (((R / dim) & 248) << 7) | (((G / dim) & 248) << 2) | ((B / dim) >> 3);
}


byte rainbow(byte i) {
  if (i > 170) return 255 - i * 3;
  if (i > 85)  return i * 3;
  return 0;
}

namespace Fluid {

void draw(int16_t matrix[HEIGHT][WIDTH]) {
  accRead();

  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      if (matrix[y][x] & 32768) continue;
      if (!matrix[y][x]) continue;

      byte dx = x + acceleration[0];    if (dx < 0 || dx >= WIDTH) continue;
      byte dy = y + acceleration[1];    if (dy < 0 || dy >= HEIGHT) continue;

      if (matrix[dy][dx]) continue;

      matrix[dy][dx] = matrix[y][x] | 32768; // Flag moved pixel
      matrix[y][x] = 32768; // Flag and blank old pixel

    }
  }

  // Clear all flags for next frame;
  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      matrix[y][x] &= ~32768;
    }
  }

  delay(30);
}

void setup(int16_t matrix[HEIGHT][WIDTH]) {
  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      randomSeed(analogRead(0) * analogRead(1) * analogRead(2));
      if (random(5) < 1) {
        byte c = random(256);
        //matrix[y][x] = (random(256) << 7) | (random(256) << 2) | (random(256) >> 3);
        matrix[y][x] = RGBsmash(rainbow(c - 170), rainbow(c - 85), rainbow(c));
      }
      else {
        matrix[y][x] = 0;
      }
    }
  }
}
}

namespace Rainbow {

void draw(int16_t matrix[HEIGHT][WIDTH]) {
  for (byte y = 0; y < HEIGHT; y++) {

    byte count = 0;

    for (byte x = 0; count < WIDTH; x++) {
      byte current = x;
      int16_t prev = matrix[y][x];
      do {
        byte next = (current + 1) % WIDTH;
        int16_t tmp = matrix[y][next];
        matrix[y][next] = prev;
        prev = tmp;
        current = next;
        count++;
      } while (x != current);
    }
  }

  //  ticks += 51;
  //  delay((32768 - abs(ticks)) / 1000);
  delay(30);
}

void setup(int16_t matrix[HEIGHT][WIDTH]) {
  for (byte y = 0; y < HEIGHT; y++) {
    byte dy = y << 2;
    for (byte x = 0; x < WIDTH; x++) {
      byte c = ((x << 3)  + dy);
      // F RRRRR GGGGG BBBBB
      matrix[y][x] = RGBsmash(rainbow(c - 170), rainbow(c - 85), rainbow(c));
      //      matrix[y][x] = RGBsmash(127, 32, 0);
    }
  }
}
}


void (*draw)(int16_t matrix[HEIGHT][WIDTH]);
void (*drawSetups[])(int16_t matrix[HEIGHT][WIDTH]) =  { Rainbow::setup, Fluid::setup };
void (*drawModes[])(int16_t matrix[HEIGHT][WIDTH]) =   { Rainbow::draw, Fluid::draw };

//////////////////////////////////////////// DISGUSTING


///////////////////////////////////////////////////////////////////////// MATRIX

void ftm0_isr(void) {
  PORTB = 0b10000;
  if (!(wait <<= 1)) {
    // 8 4 2 1
    wait = 8;     //Wait twice as long with overflow
    ++abc &= 7;
    rows[0] = abc;
    rows[1] = 8 + abc;
    rows[2] = 16 + abc;
    rows[3] = 24 + abc;
  }

  Rmask = (wait << 7);
  Gmask = (wait << 2);
  Bmask = (wait >> 3);

  uint16_t rgb0, rgb1, rgb2, rgb3;

  PORTB |= abc; // ABC↺ L↓

  // Unrolled pre-processing directives
  columnsin

  FTM0_MOD = wait << 8;    //Modulate ticks to new wait-tick amount
  FTM0_SC |= (1 << 7);       //Clear interupt flag
  FTM0_CNT = 0;         //Reset timer
  PORTB |= 8;           //Latch in -> display!
}

///////////////////////////////////////////////////////////////////////// MATRIX

void setup() {
  Serial.begin(38400);

  const uint8_t PINS[] = {  //  76543210
    2,  3,  4,  5,  6,  7,  //D BGRBGR✕✕ Row 0-1
    14, 15, 16, 17, 18, 19, //C ✕✕BGRBGR Row 2-3
    8,  9,  10, 11,         //B ✕✕✕✕LABC
    20, 13                  //⌚, OE
  };
  for (auto pin : PINS) pinMode(pin, OUTPUT);

  FTM0_SC = 0;    //Disable interupt register
  FTM0_CNT = 0;   //Reset counter
  FTM0_MOD = 256; //Set initial tick-amount to wait
  FTM0_SC = 0b011001000;
  NVIC_SET_PRIORITY(IRQ_FTM0, 128);
  NVIC_ENABLE_IRQ(IRQ_FTM0);

  accSetup(22, 23, 12);

  //  mode = EEPROM.read(0);
  //  EEPROM.write(0, ((mode+1) % (sizeof(drawModes)/sizeof(*drawModes))));

  drawSetups[mode](matrix);
  draw = drawModes[mode];
}

void loop() {
  draw(matrix);
}
