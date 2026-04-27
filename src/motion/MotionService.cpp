#ifndef MOTION_SERVICE_H
#define MOTION_SERVICE_H
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <math.h>

class MotionService {
private:
  Adafruit_MPU6050 _mpu;
  const uint8_t _addr = 0x68;

  int _stepCount = 0;
  float _threshold = 12.0f;
  float _lastMag = 0;
  bool _peakDetected = false;

public:
  bool begin() {
    if (!_mpu.begin(_addr, &Wire)) {
      return false;
    }
    _mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    _mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    _mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
    return true;
  }

  float getMagnitude() {
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);
    return sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) +
                sq(a.acceleration.z));
  }

  bool update() {
    float mag = getMagnitude();
    bool stepped = false;

    if (mag > _threshold && !_peakDetected) {
      _peakDetected = true;
      _stepCount++;
      stepped = true;
    } else if (mag < _threshold - 2.0f) {
      _peakDetected = false;
    }

    _lastMag = mag;
    return stepped;
  }

  int getStepCount() { return _stepCount; }
  void resetSteps() { _stepCount = 0; }

  void getAcceleration(float &x, float &y, float &z) {
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);
    x = a.acceleration.x;
    y = a.acceleration.y;
    z = a.acceleration.z;
  }
};
#endif