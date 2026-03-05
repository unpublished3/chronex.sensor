#pragma once
#include "MAX30105.h"

class PulseSensor {
public:
    bool begin(int sda, int scl);
    bool readSample(uint32_t& ir, uint32_t& red);
    float getTemperature();

private:
    MAX30105 sensor;
};
