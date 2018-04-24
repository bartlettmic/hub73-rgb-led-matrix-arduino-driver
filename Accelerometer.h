#include <SlowSoftI2CMaster.h>
#include "MPU6050.h"

class MPU6050 {
  public:
    int16_t raw[2];
    float   precise[2];
    SlowSoftI2CMaster si = SlowSoftI2CMaster(22, 23, true);
    void setup(byte scl, byte sda);
    void setup(void);
    void read(void);
} MPU;

void MPU6050::read() {
  si.i2c_start((MPU6050_ADDRESS_AD0_LOW << 1) | I2C_WRITE);
  si.i2c_write(MPU6050_RA_ACCEL_XOUT_H);
  si.i2c_rep_start((MPU6050_ADDRESS_AD0_LOW << 1) | I2C_READ);
  raw[1] = int16_t(si.i2c_read(false) << 8 | si.i2c_read(false)) * -1;
  raw[0] = int16_t(si.i2c_read(false) << 8 | si.i2c_read(true));
  precise[0] = float(raw[0]);
  precise[1] = float(raw[1]);
  si.i2c_stop();
}

void MPU6050::setup(void) {
  setup(22, 23);
}

void MPU6050::setup(byte scl, byte sda) {

  si = SlowSoftI2CMaster(sda, scl, true);

  si.i2c_init();

  // Wake up
  si.i2c_start((MPU6050_ADDRESS_AD0_LOW << 1) | I2C_WRITE);
  si.i2c_write(MPU6050_RA_PWR_MGMT_1);
  si.i2c_write(0);
  si.i2c_stop();

  /*  Use to get readings on interrupt INT rising (not possible with timer interupts)

    //  pinMode(inter, INPUT);

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
    attachInterrupt(digitalPinToInterrupt(inter), ACCELEROMETER_ISR, RISING);

  */

}
