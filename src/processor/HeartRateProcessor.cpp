#include "HeartRateProcessor.h"
#include "HardwareSerial.h"
#include "MAX30105.h"
#include "Print.h"
#include "heartRate.h"
#include <cstdint>

bool HeartRateProcessor::update(uint32_t irValue) {
  // --- Step 1: Maintain running min/max for dynamic normalization ---
  static uint32_t irMin = 0xFFFFFFFF;
  static uint32_t irMax = 0;

  // Update min/max
  if (irValue < irMin)
    irMin = irValue;
  if (irValue > irMax)
    irMax = irValue;

  // Avoid divide by zero
  uint32_t range = irMax - irMin;
  if (range == 0)
    range = 1;

  // --- Step 2: Normalize IR to 0-1023 ---
  int32_t normalizedIR = (int32_t)((irValue - irMin) * 1023 / range);

  // --- Step 3: Beat detection ---
  if (checkForBeat(normalizedIR)) {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60.0 / (delta / 1000.0);

    Serial.print("IR Raw: ");
    Serial.print(irValue);
    Serial.print(" | IR Norm: ");
    Serial.print(normalizedIR);
    Serial.print(" | Delta: ");
    Serial.print(delta);
    Serial.print(" | BPM: ");
    Serial.println(beatsPerMinute);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (uint8_t)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      // Moving average
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