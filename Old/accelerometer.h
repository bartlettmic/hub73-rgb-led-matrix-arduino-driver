#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include"i2c_t3.h"

const int MPU_addr = 0x68; // I2C address of the MPU-6050
//   tX, tY, +X, +Y
//   0   0   0   0   =  •
//   0   0   0   1   =  •
//   0   0   1   0   =  •
//   0   0   1   1   =  •
//   0   1   0   0   =  ←
//   0   1   0   1   =  →
//   0   1   1   0   =  ←
//   0   1   1   1   =  →
//   1   0   0   0   =  ↑
//   1   0   0   1   =  ↑
//   1   0   1   0   =  ↓
//   1   0   1   1   =  ↓
//   1   1   0   0   =  ↖
//   1   1   0   1   =  ↗
//   1   1   1   0   =  ↙
//   1   1   1   1   =  ↘

// [dx, dy]
//int8_t directions[16][2] = { {0,0}, {0,0}, {0,0}, {0,0}, {1, 0}, {-1, 0}, {1,0}, {-1,0}, {0, 1}, {0, 1}, {0, -1}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1}};


void accSetup() {
  Wire1.begin();
  Wire1.beginTransmission(MPU_addr);
  Wire1.write(0x6B);  // PWR_MGMT_1 register
  Wire1.write(0);     // set to zero (wakes up the MPU-6050)
  Wire1.endTransmission(true);
}

void accRead(int8_t* acceleration) {
  Wire1.beginTransmission(MPU_addr);
  Wire1.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire1.endTransmission(false);
  Wire1.requestFrom(MPU_addr, 4, true); // request a total of 14 registers
  byte AcX = Wire1.read() << 8 | Wire1.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  byte AcY = Wire1.read() << 8 | Wire1.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
//  byte ind = ((abs(AcX) > 4000) << 3) | ((abs(AcY) > 4000) << 2) | ((AcX < 0) << 1) | (AcY < 0);
  AcX = (AcX > 4000 ? 1 : AcX < -4000 ? -1 : 0);
//  memcpy(acceleration, directions[ind], 2 * sizeof *acceleration);
}

#endif
