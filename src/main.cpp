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

// data
BluetoothService ble;
PulseSensor sensor;
HeartRateProcessor heartRateProcessor;

uint32_t ir, red;

// put function declarations here:
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Pulse Sensor
  sensor.begin(33, 32);

  // Ble
  ble.begin("Chronex", BluetoothUuids::SERVICE);
  ble.createCharacteristic(BluetoothUuids::HEART,
                           NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  ble.startAdvertising("Chronex", BluetoothUuids::SERVICE);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (sensor.readSample(ir, red)) {
    if (heartRateProcessor.update(ir)) {
      float hr = heartRateProcessor.getBpm();
      Serial.write("BPM: ");
      Serial.write((uint32_t)hr);
      ble.sendUInt32(BluetoothUuids::HEART, hr);
    }
  }
}

// put function definitions here:
