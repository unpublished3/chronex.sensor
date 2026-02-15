#include "MAX30105.h"
#include "bluetooth/BluetoothService.h"
#include "bluetooth/BluetoothUuids.h"
#include "esp32-hal.h"
#include "sensor/PulseSensor.h"
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <Wire.h>

// data
// NimBLECharacteristic *serialChar;
// MAX30105 particleSensor;
BluetoothService ble;
PulseSensor sensor;

// put function declarations here:

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sensor.begin(33, 32);
  ble.begin("Chronex", BluetoothUuids::SERVICE);
  ble.createCharacteristic(BluetoothUuids::SERIALU,
                           NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
}

void loop() {
  // put your main code here, to run repeatedly:
  // while (sensor.) {
  
  // }
  ble.sendUInt32(BluetoothUuids::SERIALU, sensor.getIR());
  delay(1000);
}

// put function definitions here:
