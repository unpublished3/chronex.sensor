#include "HeartRateService.h"
#include "DFRobot_MAX30102.h"
#include "esp32-hal.h"

bool HeartRateService::begin() {
  if (!_sensor.begin(&Wire)) {
    return false;
  }
  _sensor.sensorConfiguration(0x1F, SAMPLEAVG_4, MODE_MULTILED,
                              SAMPLERATE_100, PULSEWIDTH_411, ADCRANGE_16384);
  return true;
}

bool HeartRateService::update() {
  _sensor.heartrateAndOxygenSaturation(&_spo2, &_spo2Valid, &_hr, &_hrValid);

  if (!_hrValid || _hr < 20 || _hr > 200) {
    return false;
  }

  _hrBuffer[_bufferIndex++] = _hr;
  _bufferIndex %= RATE_SIZE;
  if (_validCount < RATE_SIZE)
    _validCount++;

  if (_validCount >= 4) {
    int32_t sum = 0;
    for (int i = 0; i < _validCount; i++)
      sum += _hrBuffer[i];
    _smoothHr = sum / _validCount;
    return true;
  }

  return false;
}

int32_t HeartRateService::getHr() { return _smoothHr; }
int32_t HeartRateService::getSpo2() { return _spo2; }
bool HeartRateService::isSpo2Valid() { return _spo2Valid == 1; }