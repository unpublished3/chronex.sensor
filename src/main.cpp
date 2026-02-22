#include "MAX30105.h"
#include "bluetooth/BluetoothService.h"
#include "bluetooth/BluetoothUuids.h"
#include "esp32-hal.h"
#include "sensor/PulseSensor.h"
#include <Arduino.h>
#include <NimBLEDevice.h>
#include <Wire.h>

// data
BluetoothService ble;
PulseSensor sensor;

    MAX30105 sensors;

// put function declarations here:
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Pulse Sensor
  sensor.begin(33, 32);
  sensors.readTemperature();

  // Ble
  ble.begin("Chronex", BluetoothUuids::SERVICE);
  ble.createCharacteristic(BluetoothUuids::HEART,
                           NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  ble.startAdvertising("Chronex", BluetoothUuids::SERVICE);
}

void loop() {
  // put your main code here, to run repeatedly:
  ble.sendUInt32(BluetoothUuids::HEART, sensor.getIR());
  delay(1000);
}

// put function definitions here:
