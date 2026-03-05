#include "PulseSensor.h"
#include "MAX30105.h"

bool PulseSensor::begin(int sda, int scl) {
  Wire.begin(sda, scl);

  if (!sensor.begin(Wire, I2C_SPEED_FAST)) {
    return false;
  }

  sensor.setup(60, 4, 2, 100, 411, 4096);
  sensor.enableDIETEMPRDY();
  return true;
}

bool PulseSensor::readSample(uint32_t &ir, uint32_t &red) {
  sensor.check();
  if (!sensor.available())
    return false;

  ir = sensor.getIR();
  red = sensor.getRed();

  sensor.nextSample();
  return true;
}

float PulseSensor::getTemperature() { return sensor.readTemperature(); }
