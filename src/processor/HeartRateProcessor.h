#include <cstdint>

class HeartRateProcessor {
public:
  bool update(uint32_t irValue);
  float getBpm();

private:
  long lastBeat = 0;
  float beatsPerMinute = 0;
  float beatAvg = 0;

  static const uint8_t RATE_SIZE = 4;
  uint8_t rates[RATE_SIZE];
  uint8_t rateSpot = 0;
};