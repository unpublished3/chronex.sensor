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
  sensor.sensorConfiguration(60, 2, 7, 3, 3, 1);

  // Ble
  // ble.begin("Chronex", BluetoothUuids::SERVICE);
  // ble.createCharacteristic(BluetoothUuids::HEART,
  //                          NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  // ble.startAdvertising("Chronex", BluetoothUuids::SERVICE);
}

void loop() {
  // put your main code here, to run repeatedly:
  // if (sensor.readSample(ir, red)) {
  //   Serial.print("Ir: ");
  //   Serial.print(ir);
  //   Serial.print("\n");
  //   if (heartRateProcessor.update(ir)) {
  //     float hr = heartRateProcessor.getBpm();
  //     Serial.print("BPM: ");
  //     Serial.print((uint32_t)hr);
  //     ble.sendUInt32(BluetoothUuids::HEART, hr);
  //   }
  // }
  sensor.heartrateAndOxygenSaturation(&spo2, &spo2valid, &hr, &hrvalid);
  Serial.print("Hr: ");
  Serial.print(hr);
  Serial.print("\tSpo2:");
  Serial.print(spo2);
  Serial.print("\tValidHr:");
  Serial.print(hrvalid);
  Serial.print("\tValidSpo2:");
  Serial.print(spo2valid);
  Serial.print("\n");
}

// put function definitions here:
