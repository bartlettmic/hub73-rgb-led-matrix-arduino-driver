#include <SlowSoftI2CMaster.h>
#include "MPU6050.h"

SlowSoftI2CMaster si = SlowSoftI2CMaster(22, 23, true);

extern int16_t acceleration[2];

void ACCELEROMETER_ISR() {

}

void accSetup(byte scl, byte sda, byte inter) {

  si = SlowSoftI2CMaster(sda, scl, true);
  
//  pinMode(inter, INPUT);

  si.i2c_init();

  // Wake up
  si.i2c_start((MPU6050_ADDRESS_AD0_LOW << 1) | I2C_WRITE);
  si.i2c_write(MPU6050_RA_PWR_MGMT_1);
  si.i2c_write(0);
  si.i2c_stop();

  // Enable interupts
  si.i2c_start((MPU6050_ADDRESS_AD0_LOW << 1) | I2C_WRITE);
  si.i2c_write(MPU6050_RA_INT_ENABLE);
  si.i2c_write(0x32);
  si.i2c_stop();

  // Enable DMP interupt
  si.i2c_start((MPU6050_ADDRESS_AD0_LOW << 1) | I2C_WRITE);
  si.i2c_write(MPU6050_RA_USER_CTRL);
  si.i2c_write(1 << MPU6050_USERCTRL_DMP_EN_BIT);
  si.i2c_stop();

  // Attach inerupt to digital pin leading to INT
//  attachInterrupt(digitalPinToInterrupt(inter), ACCELEROMETER_ISR, RISING);

}

void accRead() {
  si.i2c_start((MPU6050_ADDRESS_AD0_LOW << 1) | I2C_WRITE);
  si.i2c_write(MPU6050_RA_ACCEL_XOUT_H);
  si.i2c_rep_start((MPU6050_ADDRESS_AD0_LOW << 1) | I2C_READ);
  acceleration[1] = int16_t(si.i2c_read(false) << 8 | si.i2c_read(false))/-4000;
  acceleration[0] = int16_t(si.i2c_read(false) << 8 | si.i2c_read(true))/3500;
  si.i2c_stop();
}


