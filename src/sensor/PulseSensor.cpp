#include "PulseSensor.h"

bool PulseSensor::begin(int sda, int scl) {
  Wire.begin(sda, scl);

  if (!sensor.begin(Wire, I2C_SPEED_FAST)) {
    return false;
  }

  sensor.setup(60, 4, 2, 100, 411, 4096);
  return true;
}

void PulseSensor::update() { sensor.check(); }

long PulseSensor::getIR() { return sensor.getIR(); }

long PulseSensor::getRed() { return sensor.getRed(); }
