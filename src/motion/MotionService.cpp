#ifndef MOTION_SERVICE_H
#define MOTION_SERVICE_H

#include <Adafruit_MPU6050.h>
#include <Wire.h>

class MotionService {
private:
  Adafruit_MPU6050 _mpu;
  const uint8_t _addr = 0x68;

public:
  bool begin() {
    if (!_mpu.begin(_addr, &Wire, 0x70)) {
      return false;
    }

    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    return true;
  }

  float getMagnitude() {
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);

    return sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) +
                sq(a.acceleration.z));
  }
};

#endif