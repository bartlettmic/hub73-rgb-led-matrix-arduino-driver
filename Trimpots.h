#pragma once

struct trimpots {
  uint16_t l, r;
  void read(void) {
    l = analogRead(20);
    r = analogRead(21);
  }
} knobs;

