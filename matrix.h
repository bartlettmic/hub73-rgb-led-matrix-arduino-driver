#ifndef MATRIX_H
#define MATRIX_H

#include "definitions.h"

#include "PWMISR_unroll.h"

volatile byte wait = 8;
volatile uint16_t Rmask = (wait << 7);
volatile uint16_t Gmask = (wait << 2);
volatile uint16_t Bmask = (wait >> 3);
volatile byte abc = 0;
volatile byte rows[4] = { 0, 8, 16, 24 };

//extern int16_t matrix[HEIGHT][WIDTH];

void matrixSetup() {
  FTM0_SC = 0;    //Disable interupt register
  FTM0_CNT = 0;   //Reset counter
  FTM0_MOD = 256; //Set initial tick-amount to wait
  FTM0_SC = 0b011001111;
  NVIC_SET_PRIORITY(IRQ_FTM0, 128);
  NVIC_ENABLE_IRQ(IRQ_FTM0);
}

extern void ftm0_isr(void) {
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

  PORTB = abc; // ABC↺ L↓

  // Unrolled pre-processing directives
  columnsin

  FTM0_MOD = wait<<1;      //Modulate ticks to new wait-tick amount
  FTM0_SC |= (1 << 7);       //Clear interupt flag
  FTM0_CNT = 0;         //Reset timer
  PORTB |= 8;           //Latch in -> display!
}

#endif
