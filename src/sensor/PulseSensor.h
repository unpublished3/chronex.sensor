#pragma once
// #include <Wire.h>
#include "MAX30105.h"

class PulseSensor {
public:
    bool begin(int sda, int scl);
    void update();
    long getIR();
    long getRed();

private:
    MAX30105 sensor;
};
