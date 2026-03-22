#include "DFRobot_MAX30102.h"
#include "HardwareSerial.h"
#include "MAX30105.h"
#include "bluetooth/BluetoothService.h"
#include "bluetooth/BluetoothUuids.h"
#include "esp32-hal.h"
#include "processor/HeartRateProcessor.h"
#include "sensor/PulseSensor.h"
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <Wire.h>
#include <cstdint>

#define RATE_SIZE 8
int32_t hrBuffer[RATE_SIZE];
int bufferIndex = 0;
int validCount = 0;

// data
// BluetoothService ble;
// PulseSensor sensor;
// HeartRateProcessor heartRateProcessor;

uint32_t ir, red;
DFRobot_MAX30102 sensor;

int32_t spo2, hr;
int8_t spo2valid, hrvalid;

// put function declarations here:

// put your setup code here, to run once:
void setup() {
  Serial.begin(9600);
  // Pulse Sensor
  // sensor.begin(33, 32);
  Wire.begin(33, 32);

  if (!sensor.begin(&Wire)) {
    Serial.println("Sensor not found");
    while (1)
      ;
  }
  sensor.sensorConfiguration(0x1F, SAMPLEAVG_16, MODE_MULTILED, SAMPLERATE_3200,
                             PULSEWIDTH_411, ADCRANGE_4096);
  // Ble
  // ble.begin("Chronex", BluetoothUuids::SERVICE);
  // ble.createCharacteristic(BluetoothUuids::HEART,
  //                          NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  // ble.startAdvertising("Chronex", BluetoothUuids::SERVICE);
}

void loop() {
  // put your main code here, to run repeatedly:

  sensor.heartrateAndOxygenSaturation(&spo2, &spo2valid, &hr, &hrvalid);
  // Serial.print(hr);
  if (hrvalid) {
    hrBuffer[bufferIndex++] = hr;
    Serial.println(hr);
    Serial.println(bufferIndex);
    Serial.println(validCount);
    bufferIndex %= RATE_SIZE;
    if (validCount < RATE_SIZE)
      validCount++; // only increment until full

    // Only average once we have enough samples
    if (validCount >= 4) { // wait for at least 4 readings
      int32_t sum = 0;
      for (int i = 0; i < validCount; i++)
        sum += hrBuffer[i];
      int32_t smoothHr = sum / validCount;
      Serial.println(smoothHr);
    } else {
      Serial.println("Collecting samples...");
    }
  } else {
    Serial.println("\nRejected value");
  }

}

// put function definitions here:
