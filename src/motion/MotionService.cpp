#pragma once

#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <cstdint>
#include <math.h>

class MotionService {
private:
  Adafruit_MPU6050 _mpu;
  const uint8_t _addr = 0x68;

  int _stepCount = 0;
  float _cadence = 0;
  float _mag = 0;

  uint8_t _millisAtPrevStep = 0;
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

  float getMagnitude() { return _mag; }

  bool update() {
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);

    float mag = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) +
                     sq(a.acceleration.z));
    bool stepped = false;

    // 1. Step Detection
    if (mag > _threshold && !_peakDetected) {
      _peakDetected = true;
      _stepCount++;
      stepped = true;

      if (_millisAtPrevStep != 0) {
        uint32_t diff = millis() - _millisAtPrevStep;
        // Only calculate if the step is physically possible (e.g. > 200ms)
        if (diff > 200 && diff < 3000) {
          float instantCadence = 60000.0f / diff;
          _cadence = (_cadence * 0.7f) + (instantCadence * 0.3f);
        }
      }
      _millisAtPrevStep = millis();
    }

    // 2. Reset Peak Detection (Hysteresis)
    // Bring this up slightly to ensure it resets even with noise
    if (mag < _threshold - 1.0f) {
      _peakDetected = false;
    }

    // 3. Independent Timeout (Place this OUTSIDE the step detection)
    if (millis() - _millisAtPrevStep > 2500) {
      _cadence = 0.0f;
    }

    return stepped;
  }

  int getStepCount() { return _stepCount; }
  int getCadence() { return _millisAtPrevStep; }
  void resetSteps() { _stepCount = 0; }

  void getAcceleration(float &x, float &y, float &z) {
    sensors_event_t a, g, temp;
    _mpu.getEvent(&a, &g, &temp);
    x = a.acceleration.x;
    y = a.acceleration.y;
    z = a.acceleration.z;
  }
};
