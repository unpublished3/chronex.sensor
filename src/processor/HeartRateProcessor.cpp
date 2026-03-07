#include "HeartRateProcessor.h"
#include "HardwareSerial.h"
#include "MAX30105.h"
#include "Print.h"
#include "heartRate.h"
#include <cstdint>

bool HeartRateProcessor::update(uint32_t irValue) {
  Serial.print("Update tried!\n");

  if (checkForBeat(irValue)) {
    Serial.print("Beat Found!!\n");
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (uint8_t)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (uint8_t x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];

      beatAvg /= RATE_SIZE;
      return true;
    }
  }

  return false;
}

float HeartRateProcessor::getBpm() { return beatsPerMinute; }