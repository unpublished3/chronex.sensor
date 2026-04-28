#include "DFRobot_MAX30102.h"
#include "HardwareSerial.h"
#include "MAX30105.h"
#include "NimBLELocalValueAttribute.h"
#include "bluetooth/BluetoothService.h"
#include "bluetooth/BluetoothUuids.h"
#include "esp32-hal.h"
#include "heartrate/HeartRateService.h"
#include "motion/MotionService.cpp"
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <Wire.h>
#include <cstdint>

// data
BluetoothService ble;
HeartRateService heartRateService;
MotionService motionService;

// put function declarations here:

// put your setup code here, to run once:
void setup() {
  Serial.begin(9600);
  delay(2000);
  Wire.begin(33, 32);

  // Sensor Initialization
  if (!heartRateService.begin()) {
    Serial.println("Hr sensor not found!");
    ;
    while (1)
      ;
  }

  if (!motionService.begin()) {
    Serial.println("Motion sensor not found!");
    ;
    while (1)
      ;
  }

  // // Ble
  ble.begin("Chronex", BluetoothUuids::SERVICE);
  ble.createCharacteristic(BluetoothUuids::HEART,
                           NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  ble.createCharacteristic(BluetoothUuids::MOTION, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  ble.startAdvertising("Chronex", BluetoothUuids::SERVICE);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (motionService.update()) {
    ble.sendMotionData(BluetoothUuids::MOTION, motionService.getStepCount(), motionService.getCadence());
  }

  if (heartRateService.update()) {
    int32_t hr = heartRateService.getHr();
    Serial.println(hr);
    ble.sendHeartRate(BluetoothUuids::HEART, hr);
  }
}

// put function definitions here:
