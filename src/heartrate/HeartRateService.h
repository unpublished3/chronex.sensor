#pragma once
#include <DFRobot_MAX30102.h>
#include <Wire.h>

#define RATE_SIZE 8

class HeartRateService {
public:
    bool begin(int sda, int scl);
    bool update();
    int32_t getHr();
    int32_t getSpo2();
    bool isSpo2Valid();

private:
    DFRobot_MAX30102 _sensor;
    int32_t _hr = 0;
    int32_t _spo2 = 0;
    int8_t _hrValid = 0;
    int8_t _spo2Valid = 0;

    int32_t _hrBuffer[RATE_SIZE] = {0};
    int _bufferIndex = 0;
    int _validCount = 0;
    int32_t _smoothHr = 0;
};