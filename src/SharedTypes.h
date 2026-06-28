#pragma once
#include <cstdint>

typedef struct __attribute__((packed)) {
  uint32_t steps;
  float cadence;
} MotionData;
