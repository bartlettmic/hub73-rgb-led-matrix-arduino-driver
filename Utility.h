#pragma once
extern int16_t matrix[HEIGHT][WIDTH];

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

int16_t rainbowcycle(byte c) {
  return RGBsmash(rainbow(c), rainbow(c - 85), rainbow(c - 170));
}

static inline int8_t sgn(int val) {
  if (val < 0) return -1;
  return 1;
}

void fillMatrix(int16_t color) {
  memset(matrix, color, sizeof(matrix));
}

void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const int16_t color) {
  int32_t dy = y1 - y0, dx = x1 - x0, gradient, x, y, shift = 0x10;

  if (abs(dx) > abs(dy)) {
    // x variation is bigger than y variation
    if (x1 < x0) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }
    gradient = (dy << shift) / dx ;

    for (x = x0 , y = 0x8000 + (y0 << shift); x <= x1; ++x, y += gradient) {
      matrix[y >> shift][x] = color;
    }
  } else if (dy != 0) {
    // y variation is bigger than x variation
    if (y1 < y0) {
      std::swap(x0, x1);
      std::swap(y0, y1);
    }
    gradient = (dx << shift) / dy;
    for (y = y0 , x = 0x8000 + (x0 << shift); y <= y1; ++y, x += gradient) {
      matrix[y][x >> shift] = color;
    }
  } else {
    matrix[y0][x0] = color;
  }
}

void pixel(uint8_t x, uint8_t y, int16_t color) {         //Set pixel in buffer safely
  if (x >= WIDTH || x < 0 || y < 0 || y >= HEIGHT) return;
  matrix[y][x] = color;
}

void circle(uint8_t x0, uint8_t y0, uint8_t radius, int16_t color) {
  uint8_t x = radius, y = 0;
  int8_t radiusError = 1 - x;

  while (y <= x) {
    pixel(x + x0, y + y0, color);
    pixel(y + x0, x + y0, color);
    pixel(-x + x0, y + y0, color);
    pixel(-y + x0, x + y0, color);
    pixel(-x + x0, -y + y0, color);
    pixel(-y + x0, -x + y0, color);
    pixel(x + x0, -y + y0, color);
    pixel(y + x0, -x + y0, color);
    y++;
    if (radiusError < 0) radiusError += 2 * y + 1;
    else radiusError += 2 * (y - --x + 1);
  }
}

void fillCircle(uint8_t x0, uint8_t y0, uint8_t radius, int16_t color) {
  while (radius > 0) circle(x0, y0, radius--, color);
  pixel(x0, y0, color);
}


void fillSpiral(int16_t color, uint8_t t_delay, bool outward = true, bool complete = true) {

  //https://stackoverflow.com/questions/1183013/spiral-algorithm-in-c-sharp#1183030

  if (outward) {

    int8_t x = WIDTH / 2, y = HEIGHT / 2, size = 0;

    uint8_t limit = complete ? WIDTH : WIDTH - 1;

    while (size <= limit)  {

      for (int i = y; i <= y + size - 1; i++)     matrix[i][x]            = color;
      delay(t_delay);
      for (int j = x + 1; j <= x + size - 1; j++) matrix[y + size - 1][j] = color;
      delay(t_delay);
      for (int i = y + size - 2; i >= y; i--)     matrix[i][x + size - 1] = color;
      delay(t_delay);
      for (int i = x + size - 2; i >= x + 1; i--) matrix[y][i]            = color;
      delay(t_delay);

      x--;    y--;    size += 2;
    }
  }

  else {

    int8_t x = complete ? 0 : 1, y = complete ? 0 : 1, size = complete ? WIDTH : WIDTH - 2;

    uint8_t limit = complete ? 0 : 1;

    while (size > limit)  {

      for (int i = y; i <= y + size - 1; i++)     matrix[i][x]            = color;
      delay(t_delay);
      for (int j = x + 1; j <= x + size - 1; j++) matrix[y + size - 1][j] = color;
      delay(t_delay);
      for (int i = y + size - 2; i >= y; i--)     matrix[i][x + size - 1] = color;
      delay(t_delay);
      for (int i = x + size - 2; i >= x + 1; i--) matrix[y][i]            = color;
      delay(t_delay);

      x++;    y++;    size -= 2;
    }
  }
}


