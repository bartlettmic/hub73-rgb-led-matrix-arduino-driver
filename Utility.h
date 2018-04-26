#pragma once
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

void spiralFill(int16_t matrix[HEIGHT][WIDTH], int16_t color, uint8_t t_delay, bool outward = true, bool complete = true) {

  //https://stackoverflow.com/questions/1183013/spiral-algorithm-in-c-sharp#1183030

  if (outward) {

    int8_t x = WIDTH / 2, y = HEIGHT / 2, size = 0;

    uint8_t limit = complete ? WIDTH : WIDTH-1;

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
    
    int8_t x = complete ? 0 : 1, y = complete ? 0 : 1, size = complete ? WIDTH : WIDTH-2;

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


